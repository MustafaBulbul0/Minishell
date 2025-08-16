/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:54 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:54:58 by esir             ###   ########.fr       */
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

static void	handle_directory_command(char *cmd_name, t_cmd *all_commands,
		t_token *all_tokens, t_envlist *env)
{
	struct stat	path_stat;

	if (stat(cmd_name, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": Is a directory\n", 17);
		free_commands(all_commands);
		free_tokens(all_tokens);
		free_env(env);
		exit(126);
	}
}

static char	*resolve_command_path(t_cmd *cmd, t_all *all_struct)
{
	char	*path;

	if (!cmd->cmd || !*(cmd->cmd))
	{
		free_t_all(all_struct);
		exit(0);
	}
	if (ft_strchr(cmd->cmd, '/'))
	{
		handle_directory_command(cmd->cmd, all_struct->all_commands,
			all_struct->all_tokens, all_struct->env);
		path = ft_strdup(cmd->cmd);
	}
	else
		path = get_exec_path(cmd->cmd, all_struct->env);
	if (!path)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": command not found\n", 20);
		free_t_all(all_struct);
		exit(127);
	}
	return (path);
}

void	execute_external_command(t_cmd *cmd, t_all *all_struct)
{
	char	*path;
	char	**envp;
	int		saved_errno;

	path = resolve_command_path(cmd, all_struct);
	envp = envlist_to_array(all_struct->env);
	execve(path, cmd->args, envp);
	saved_errno = errno;
	write(2, "minishell: ", 11);
	write(2, cmd->cmd, ft_strlen(cmd->cmd));
	write(2, ": ", 2);
	write(2, strerror(saved_errno), ft_strlen(strerror(saved_errno)));
	write(2, "\n", 1);
	free(path);
	ft_free_split(envp);
	free_t_all(all_struct);
	if (saved_errno == EACCES)
		exit(126);
	exit(127);
}
