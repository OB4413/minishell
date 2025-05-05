/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/04 16:42:09 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_redirection(t_command *cmd, list_env **env_list, char ***env)
{
    int (fd);
    int flags;

    t_command *cmd2 = cmd;
    if(cmd2->inoutfile && (cmd2->inoutfile->type == 1 || cmd2->inoutfile->type == 2))
    {
        // printf("wach\n");
        // exit(1);
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
        // write(fd, cmd2->cmd, ft_strlen(cmd2->cmd));
        // i = -1;
        // while(cmd2->args[++i])
        //     write(fd, cmd2->args[i], ft_strlen(cmd2->args[i]));
        // exit(1);
        return (1);
    }
    else if(cmd2->inoutfile && (cmd2->inoutfile->type == 0 || cmd2->inoutfile->type == 3))
        return(printf("bach\n"),in_heredoc_redirs(cmd), exit(1), 1);
    else
        return (printf("3lach\n"),0);
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

void in_heredoc_redirs(struct s_command *cmd)
{
    int fd;
    if(cmd->inoutfile->type == 0)
    {
        fd = open(cmd->inoutfile->filename, O_RDONLY);
        if(fd == -1)
            return(perror("open REDIRECT_IN"));
        dup2(fd, 0);
        close(fd);
    }
    else
        heredoc_redirection(cmd);
}
