/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:58 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 21:34:45 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

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

static void	free_redirections(t_redirection *redirs)
{
	t_redirection	*tmp;

	while (redirs)
	{
		tmp = redirs;
		redirs = redirs->next;
		if (tmp->infile)
			free(tmp->infile);
		if (tmp->outfile)
			free(tmp->outfile);
		free(tmp);
	}
}

void	free_commands(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		if (tmp->cmd)
			free(tmp->cmd);
		ft_free_split(tmp->args);
		free_redirections(tmp->redirections);
		free(tmp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}

void	free_env(t_envlist *env)
{
	t_envlist	*current;
	t_envlist	*next_node;

	current = env;
	while (current)
	{
		next_node = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next_node;
	}
}

void	free_all(t_cmd *cmd_list, t_token *tokens, t_envlist *env, t_redirection *redirections)
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
