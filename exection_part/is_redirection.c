/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 10:40:59 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/03 16:35:26 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_redirection(t_command *cmd, list_env **env_list, char ***env)
{
    int (fd), (i);
    int flags;
    
    printf("entered is redir\n");
    t_command *cmd2 = cmd;
        // exit(0);
    if(cmd2->inoutfile == NULL){
        printf("hhhhhhhhhhhhh\n");
    }
    // int j = 1;
	// t_command *cmd_list2 = cmd;
	// while (cmd_list2)
	// {
	// 		// printf("command %d:\n", j);
	// 		// printf("cmd :%s\n", cmd_list2->cmd);
	// 		// printf("args :");
	// 		// for (size_t i = 0; cmd_list2->args[i]; i++)
	// 		// {
	// 		// 	printf("%s  ", cmd_list2->args[i]);
	// 		// }
	// 		// printf("\n");
    //         // if (cmd_list2->inoutfile == NULL) 
    //         //         exit(1);
	// 		// while(cmd_list2->inoutfile)
	// 		// {
               
	// 		// 	printf("filename :%s   type:%d\n",  cmd_list2->inoutfile->filename, cmd_list2->inoutfile->type);
	// 		// 	cmd_list2->inoutfile = cmd_list2->inoutfile->next;
	// 		// }
	// 		cmd_list2 = cmd_list2->next;
	// 		j++;
	// }
    // printf("filename :%s   type:%d\n",  cmd2->inoutfile->filename, cmd2->inoutfile->type);
    //     exit(0);
    if(cmd2->inoutfile && (cmd2->inoutfile->type == 1 || cmd2->inoutfile->type == 2))
    {
        printf("wach\n");
        exit(1);
        if(cmd2->inoutfile->type == 1)
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        else
            flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd2->inoutfile->filename, flags, 0644);
        if (fd == -1)
            return(perror("fd error :"), 1);
        dup2(fd, 1);
        execute_cmd(cmd, env_list, env);
        write(fd, cmd2->cmd, ft_strlen(cmd2->cmd));
        i = -1;
        while(cmd2->args[++i])
            write(fd, cmd2->args[i], ft_strlen(cmd2->args[i]));
        close(fd);
        return (1);
    }
    else if(cmd2->inoutfile && (cmd2->inoutfile->type == 0 || cmd2->inoutfile->type == 3))
        return(printf("bach\n"),in_heredoc_redirs(cmd), 1);
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
