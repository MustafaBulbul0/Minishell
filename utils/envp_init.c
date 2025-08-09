/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:43 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:17:44 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static t_envlist	*create_env_node(char *env_string);
static void			add_node(t_envlist **list, t_envlist *new_node);

t_envlist	*envp_init(char **envp)
{
	t_envlist	*list;
	t_envlist	*new_node;
	int			i;

	list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
			return (NULL);
		add_node(&list, new_node);
		i++;
	}
	return (list);
}

static t_envlist	*create_env_node(char *env_string)
{
	t_envlist	*node;
	char		*key;
	char		*value;
	int			i;

	i = 0;
	while (env_string[i] && env_string[i] != '=')
		i++;
	key = ft_strndup(env_string, i);
	if (env_string[i] == '=')
		value = ft_strdup(env_string + i + 1);
	else
		value = NULL;
	node = malloc(sizeof(t_envlist));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	if (env_string[i] == '=')
		node->eq = 1;
	else
		node->eq = 0;
	node->next = NULL;
	return (node);
}

static void	add_node(t_envlist **list, t_envlist *new_node)
{
	t_envlist	*current;

	if (!new_node)
		return ;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
}

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
		return (0);
	return (ft_strcmp(cmd->cmd, "cd") == 0
		|| ft_strcmp(cmd->cmd, "echo") == 0
		|| ft_strcmp(cmd->cmd, "env") == 0
		|| ft_strcmp(cmd->cmd, "exit") == 0
		|| ft_strcmp(cmd->cmd, "pwd") == 0
		|| ft_strcmp(cmd->cmd, "unset") == 0
		|| ft_strcmp(cmd->cmd, "export") == 0);
}
