/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:54 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 00:50:11 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	handle_directory_command(char *cmd_name, t_cmd *all_commands, t_token *all_tokens, t_envlist *env)
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

void	execute_external_command(t_cmd *cmd, t_envlist *env, t_cmd *all_commands, t_token *all_tokens)
{
	char	*path;
	char	**envp;
	int		saved_errno;

	if (!cmd->cmd || !*(cmd->cmd))
		exit(0);
	if (ft_strchr(cmd->cmd, '/'))
	{
		handle_directory_command(cmd->cmd, all_commands, all_tokens, env);
		path = ft_strdup(cmd->cmd);
	}
	else
		path = get_exec_path(cmd->cmd, env);
	if (!path)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": command not found\n", 20);
		free_commands(all_commands);
		free_tokens(all_tokens);
		free_env(env);
		exit(127);
	}
	envp = envlist_to_array(env);
	execve(path, cmd->args, envp);
	saved_errno = errno;
	write(2, "minishell: ", 11);
	write(2, cmd->cmd, ft_strlen(cmd->cmd));
	write(2, ": ", 2);
	write(2, strerror(saved_errno), ft_strlen(strerror(saved_errno)));
	write(2, "\n", 1);
	free(path);
	ft_free_split(envp);
	free_commands(all_commands);
	free_tokens(all_tokens);
	free_env(env);
	if (saved_errno == EACCES)
		exit(126);
	exit(127);
}
