/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:37:23 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:38:00 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_last_exit = 130;
}

int	input_control(char *input)
{
	if (!input)
	{
		printf("exit\n");
		g_last_exit = 1;
		return (1);
	}
	else if (!(*input))
	{
		free(input);
		return (2);
	}
	return (0);
}

void	signals(int argc, char **argv)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
}

void	execute_builtin(t_cmd *cmd, t_envlist *env, int is_child)
{
	if (!cmd || !cmd->cmd)
		return ;
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		builtin_env(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		builtin_exit(cmd, is_child);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		builtin_cd(cmd->args, env);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		builtin_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		builtin_export(env, cmd->args);
}

void	prepare_child_process(t_cmd *cmd, int in_fd, int *pipe_fd)
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
