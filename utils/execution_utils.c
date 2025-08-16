/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:51 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/16 14:47:41 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env_nodes(t_envlist *env)
{
	int			count;
	t_envlist	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	**allocate_env_array(int count)
{
	char	**arr;

	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	return (arr);
}

static char	*create_env_string(t_envlist *node)
{
	if (node->value)
		return (ft_strjoin_three(node->key, "=", node->value));
	else
		return (ft_strjoin_three(node->key, "=", ""));
}

static void	free_partial_array(char **arr, int i)
{
	while (i > 0)
	{
		i--;
		free(arr[i]);
	}
	free(arr[i]);
	free(arr);
}

char	**envlist_to_array(t_envlist *env)
{
	int			i;
	int			count;
	t_envlist	*tmp;
	char		**arr;

	count = count_env_nodes(env);
	arr = allocate_env_array(count);
	if (!arr)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		arr[i] = create_env_string(tmp);
		if (!arr[i])
		{
			free_partial_array(arr, i);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}
