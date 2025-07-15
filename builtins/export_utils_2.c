#include "../minishell.h"

static t_envlist	*init_env_node_base(t_envlist *node)
{
	t_envlist	*new;

	new = malloc(sizeof(t_envlist));
	if (!new)
		return (NULL);
	new->key = ft_strdup(node->key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static t_envlist	*create_new_env_node(t_envlist *node)
{
	t_envlist	*new;

	new = init_env_node_base(node);
	if (!new)
		return (NULL);
	if (node->value)
	{
		new->value = ft_strdup(node->value);
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	new->eq = node->eq;
	new->next = NULL;
	return (new);
}

static void	update_node_value(t_envlist *target, t_envlist *source)
{
	if (source->eq)
	{
		if (target->value)
			free(target->value);
		if (source->value)
			target->value = ft_strdup(source->value);
		else
			target->value = NULL;
	}
	target->eq = source->eq;
}

void	add_or_update_env(t_envlist *env, t_envlist *node)
{
	t_envlist	*curr;
	t_envlist	*new;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, node->key) == 0)
		{
			update_node_value(curr, node);
			return ;
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	new = create_new_env_node(node);
	if (!new)
		return ;
	curr->next = new;
}
