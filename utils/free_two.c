/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_two.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:00:25 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 14:35:22 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	free_t_all(t_all *all)
{
	if (!all)
		return ;
	if (all->all_commands)
		free_commands(all->all_commands);
	if (all->env)
		free_env(all->env);
	if (all->all_tokens)
		free_tokens(all->all_tokens);
	free(all);
	all = NULL;
}

void	free_all(t_cmd *cmd_list, t_token *tokens,
			t_envlist *env, t_redirection *redirections)
{
	if (cmd_list)
		free_commands(cmd_list);
	if (tokens)
		free_tokens(tokens);
	if (env)
		free_env(env);
	if (redirections)
		free_redirections(redirections);
}

void	ft_free_split(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	print_redir_error(char *filename)
{
	write(2, "minishell: ", 11);
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}
