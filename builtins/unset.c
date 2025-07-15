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
