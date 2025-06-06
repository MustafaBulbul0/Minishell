#include "../minishell.h"

static void	add_or_update_env(t_envlist *env, t_envlist *node);
static void	sort_envlist(t_envlist *list);
static void	swap_env_content(t_envlist *a, t_envlist *b);
static void	print_export_format(t_envlist *list);

void	ft_export(t_envlist *env, char **arg)
{
	t_envlist	*node;
	t_envlist	*next;

	if (!arg[1])
	{
		sort_envlist(env);
		print_export_format(env);
		return ;
	}
	node = envp_init(arg + 1);
	if (!node)
		return ;
	while (node)
	{
		next = node->next;
		add_or_update_env(env, node);
		node = next;
	}
	sort_envlist(env);
}

static t_envlist	*create_new_env_node(t_envlist *node)
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
	new->next = NULL;
	return (new);
}

static void	add_or_update_env(t_envlist *env, t_envlist *node)
{
	t_envlist	*curr;
	t_envlist	*new;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, node->key) == 0)
		{
			if (curr->value)
				free(curr->value);
			if (node->value)
				curr->value = ft_strdup(node->value);
			else
				curr->value = NULL;
			curr->eq = node->eq;
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

static void	sort_envlist(t_envlist *list)
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

static void	print_export_format(t_envlist *list)
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
