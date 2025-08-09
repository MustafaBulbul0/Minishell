/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:42 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 15:36:25 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	wait_for_children(pid_t last_pid)
{
	int	exit_status;

	if (last_pid == -1)
		return ;
	exit_status = get_last_process_status(last_pid);
	while (wait(NULL) > 0)
		;
	g_last_exit = exit_status;
}

static void	close_unused_fds(int in_fd, int *pipe_fd, t_cmd *curr)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (curr->next)
	{
		close(pipe_fd[1]);
	}
}

static void	prepare_child_process(t_cmd *cmd, int in_fd, int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	execute_child_process(t_cmd *cmd, int in_fd,
		int *pipe_fd, t_envlist *env)
{
	prepare_child_process(cmd, in_fd, pipe_fd);
	if (handle_redirections_fd(cmd) != 0)
		exit(1);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env, 1);
		exit(g_last_exit);
	}
	else
		execute_external_command(cmd, env);
	exit(1);
}

void	execute_pipeline(t_cmd *cmd, t_envlist *env)
{
	int		in_fd;
	int		pipe_fd[2];
	pid_t	pid;
	t_cmd	*curr;

	in_fd = STDIN_FILENO;
	curr = cmd;
	pid = -1;
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == 0)
			execute_child_process(curr, in_fd, pipe_fd, env);
		close_unused_fds(in_fd, pipe_fd, curr);
		if (curr->next)
			in_fd = pipe_fd[0];
		curr = curr->next;
	}
	wait_for_children(pid);
}
