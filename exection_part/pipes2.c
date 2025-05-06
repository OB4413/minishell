/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:07:07 by ael-jama          #+#    #+#             */
/*   Updated: 2025/05/06 10:40:38 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void    handle_parent_process(int *prev_fd, int *pipe_fd, pid_t pid,
    int *status)
{
    if (pipe_fd)
        close(pipe_fd[1]);
    if (*prev_fd != -1)
        close(*prev_fd);
    if (pipe_fd)
        *prev_fd = pipe_fd[0];
    else
        *prev_fd = -1;
    waitpid(pid, status, 0);
}

static int    setup_pipe(int pipe_fd[2], t_env_lst *list)
{
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (0);
    }
    if (!list->next)
    {
        perror("minishell: parse error near `|'\n");
        return (0);
    }
    return (1);
}

static int    create_process(pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        perror("fork");
        return (0);
    }
    return (1);
}

static void    update_exit_status(int status)
{
    if (WIFEXITED(status))
        data_struc()->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        data_struc()->exit_status = 128 + WTERMSIG(status);
    else
        data_struc()->exit_status = 1;
}

static void    process_command(t_string *st_string, t_env_lst **list, int *prev_fd,
    int *status)
{
    char    **args;
    pid_t    pid;
    int        pipe_fd[2];
    int        *child_pipe_fd;
    int        *parent_pipe_fd;

    args = git_array(list);
    if (!args)
        return ;
    if (*list && (*list)->type == PIPE)
    {
        if (!setup_pipe(pipe_fd, *list))
            return ;
        *list = (*list)->next;
    }
    if (!create_process(&pid))
        return ;
    if (pid == 0)
    {
        child_pipe_fd = NULL;
        if (*list)
            child_pipe_fd = pipe_fd;
        handle_child_process(args, *prev_fd, child_pipe_fd, st_string);
    }
    else
    {
        assign_cmd_signals();
        parent_pipe_fd = NULL;
        if (*list)
            parent_pipe_fd = pipe_fd;
        handle_parent_process(prev_fd, parent_pipe_fd, pid, status);
        assign_signals_handler();
    }
}

void    execute_pipeline(t_string *st_string)
{
    int            prev_fd;
    t_env_lst    *list;
    int            status;

    prev_fd = -1;
    status = 0;
    if (!st_string->head)
        return ;
    list = st_string->head;
    while (list)
    {
        process_command(st_string, &list, &prev_fd, &status);
        if (!list)
            break ;
    }
    if (prev_fd != -1)
        close(prev_fd);
    update_exit_status(status);
}