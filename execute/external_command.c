/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:54 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 16:00:04 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	handle_execve_failure(t_cmd *cmd, char *path, char **envp)
{
	int	saved_errno;

	saved_errno = errno;
	write(2, "minishell: ", 11);
	write(2, cmd->cmd, ft_strlen(cmd->cmd));
	write(2, ": ", 2);
	write(2, strerror(saved_errno), ft_strlen(strerror(saved_errno)));
	write(2, "\n", 1);
	free(path);
	ft_free_split(envp);
	if (saved_errno == EACCES)
		exit(126);
	exit(127);
}

static void	handle_path_not_found(char *cmd_name)
{
	write(2, cmd_name, ft_strlen(cmd_name));
	write(2, ": command not found\n", 20);
	exit(127);
}

static void	handle_directory_command(char *cmd_name)
{
	struct stat	path_stat;

	if (stat(cmd_name, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": Is a directory\n", 17);
		exit(126);
	}
}

void	execute_external_command(t_cmd *cmd, t_envlist *env)
{
	char	*path;
	char	**envp;

	if (!cmd->cmd || !*(cmd->cmd))
		exit(0);
	if (ft_strchr(cmd->cmd, '/'))
	{
		handle_directory_command(cmd->cmd);
		path = ft_strdup(cmd->cmd);
	}
	else
		path = get_exec_path(cmd->cmd, env);
	if (!path)
	{
		handle_path_not_found(cmd->cmd);
	}
	envp = envlist_to_array(env);
	execve(path, cmd->args, envp);
	handle_execve_failure(cmd, path, envp);
}
