/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/06 15:59:00 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_redirection(t_command *cmd, list_env **env_list, char ***env)
{
    int (fd), (flags), (saved_stdout);
    saved_stdout = dup(1);

    t_command *cmd2 = cmd;
    if(cmd2->inoutfile && (cmd2->inoutfile->type == 1 || cmd2->inoutfile->type == 2))
    {
        if(cmd2->inoutfile->type == 1)
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        else
            flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd2->inoutfile->filename, flags, 0644);
        if (fd == -1)
            return(perror("fd error :"), 1);
        if(dup2(fd, 1) == -1)
            return(perror("dup2 failed"), 0);
        close(fd);
        execute_cmd(cmd, env_list, env);
        dup2(saved_stdout, 1);
        close(saved_stdout);
        return (1);
    }
    else if(cmd2->inoutfile && (cmd2->inoutfile->type == 0 || cmd2->inoutfile->type == 3))
        return(in_heredoc_redirs(cmd, env_list, env), 1);
    else
        return (0);
}

void heredoc_redirection(struct s_command *cmd)
{
    char *line;
    int pipefd[2];
    exit(1);
    line = NULL;
    if(pipe(pipefd) == -1)
        return(perror("pipe"));
    while(1)
    {
        line = readline("> ");
		if (!line)
			break;
        if (ft_strncmp(line, cmd->inoutfile->filename, strlen(cmd->inoutfile->filename)) == 0 &&
            line[strlen(cmd->inoutfile->filename)] == '\n')
            break;
        write(pipefd[1], line, strlen(line));
    }
    free(line);
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

void in_heredoc_redirs(struct s_command *cmd, list_env **env_list, char ***env)
{
    int fd;
    if(cmd->inoutfile->type == 0)
    {
        fd = open(cmd->inoutfile->filename, O_RDONLY);
        if(fd == -1)
            return(perror("open REDIRECT_IN"));
        dup2(fd, 0);
        execute_cmd(cmd, env_list, env);
        close(fd);
    }
    else
        heredoc_redirection(cmd);
}

void execute_piped_commands(t_command *cmd_list, list_env **env_list, char ***env)
{
    pid_t pid;
    int (pipe_fd[2]), (prev_fd);
    prev_fd = -1;
    if(!cmd_list->next){
        return(execute_cmd(cmd_list, env_list, env));
    }
    while (cmd_list)
    {
        if (cmd_list->next) // not the last command: create a pipe
        {
            if (pipe(pipe_fd) == -1)
                return (perror("pipe"));
        }
        pid = fork();
        if (pid == -1)
            return (perror("fork"));
        if (pid == 0) // Child process
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if (cmd_list->next)
            {
                dup2(pipe_fd[1], 1);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            if(is_redirection(cmd_list, env_list, env) != 1)
                execute_cmd(cmd_list, env_list, env);
            exit(EXIT_FAILURE);
        }
        else // Parent process
        {
            waitpid(pid, NULL, 0);

            if (prev_fd != -1)
                close(prev_fd);
            if (cmd_list->next)
            {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
            cmd_list = cmd_list->next;
        }
    }
}
