/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:42 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:37:41 by mubulbul         ###   ########.fr       */
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

static void	execute_child_process(t_cmd *cmd, int in_fd,
		int *pipe_fd, t_all *all_struct)
{
	int	status;

	prepare_child_process(cmd, in_fd, pipe_fd);
	if (handle_redirections_fd(cmd) != 0)
	{
		free_t_all(all_struct);
		exit(1);
	}
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, all_struct->env, 1);
		status = g_last_exit;
		free_t_all(all_struct);
		exit(status);
	}
	else
	{
		execute_external_command(cmd, all_struct);
		free_t_all(all_struct);
		exit(127);
	}
}

static pid_t	spawn_child_process(t_cmd *curr, int in_fd, int pipe_fd[2],
		t_all *all)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process(curr, in_fd, pipe_fd, all);
	return (pid);
}

void	execute_pipeline(t_cmd *cmd, t_envlist *env, t_cmd *all_commands,
			t_token *all_tokens)
{
	int		in_fd;
	int		pipe_fd[2];
	pid_t	pid;
	t_cmd	*curr;
	t_all	*all;

	in_fd = STDIN_FILENO;
	curr = cmd;
	pid = -1;
	all = all_struct(all_commands, env, all_tokens);
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) == -1)
		{
			if (pid == -1)
				return ;
			break ;
		}
		pid = spawn_child_process(curr, in_fd, pipe_fd, all);
		close_unused_fds(in_fd, pipe_fd, curr);
		if (curr->next)
			in_fd = pipe_fd[0];
		curr = curr->next;
	}
	wait_for_children(pid);
}
