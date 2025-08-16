/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:36 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 15:11:50 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	remove_node(t_envlist **list, char *key);

void	builtin_unset(t_envlist *list, char **arg)
{
	int		i;

	if (!arg || !arg[1])
		return ;
	i = 1;
	while (arg[i])
	{
		remove_node(&list, arg[i]);
		i++;
	}
	g_last_exit = 0;
}

static void	remove_node(t_envlist **list, char *key)
{
	t_envlist	*curr;
	t_envlist	*prev;

	curr = *list;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*list = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
