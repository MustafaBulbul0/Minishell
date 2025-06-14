#include "../minishell.h"

static void	swap_env_content(t_envlist *a, t_envlist *b)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = a->key;
	tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

void	sort_envlist(t_envlist *list)
{
	t_envlist	*curr;
	t_envlist	*end;
	int			sorted;

	end = NULL;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		curr = list;
		while (curr && curr->next != end)
		{
			if (ft_strcmp(curr->key, curr->next->key) > 0)
			{
				swap_env_content(curr, curr->next);
				sorted = 0;
			}
			curr = curr->next;
		}
		end = curr;
	}
}

void	print_export_format(t_envlist *list)
{
	while (list)
	{
		printf("declare -x %s", list->key);
		if (list->value)
			printf("=\"%s\"", list->value);
		printf("\n");
		list = list->next;
	}
}
