/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:42 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:55:59 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

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

static void	run_pipeline(t_cmd *cmd, t_all *all)
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

void	execute_pipeline(t_cmd *cmd, t_envlist *env,
			t_cmd *all_commands, t_token *all_tokens)
{
	t_all	*all;

	all = all_struct(all_commands, env, all_tokens);
	if (!all)
		return ;
	run_pipeline(cmd, all);
	free(all);
}
