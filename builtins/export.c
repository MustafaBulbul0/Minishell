#include "../minishell.h"

void	free_envlist(t_envlist *list);

static int	is_valid_env_key(const char *key)
{
	if (!key || *key == '\0')
		return (0);

	if (!ft_isalpha(*key) && *key != '_')
		return (0);

	key++;
	while (*key)
	{
		if (!ft_isalnum(*key) && *key != '_')
			return (0);
		key++;
	}
	return (1);
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
	new->eq = node->eq;
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
			if (node->eq)
			{
				if (curr->value)
					free(curr->value);
				if (node->value)
					curr->value = ft_strdup(node->value);
				else
					curr->value = NULL;
			}
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

static t_envlist	*parse_env_string_to_node(const char *s)
{
	t_envlist	*node;
	char		*eq_pos;

	node = malloc(sizeof(t_envlist));
	if (!node)
		return (NULL);
	eq_pos = ft_strchr(s, '=');
	if (eq_pos)
	{
		node->key = ft_substr(s, 0, eq_pos - s);
		node->value = ft_strdup(eq_pos + 1);
		node->eq = 1;
	}
	else
	{
		node->key = ft_strdup(s);
		node->value = NULL;
		node->eq = 0;
	}
	if (!node->key || (eq_pos && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static int	handle_single_export_argument(t_envlist *env, const char *arg_str)
{
	t_envlist	*temp_node;
	int			ret;

	temp_node = parse_env_string_to_node(arg_str);
	if (!temp_node)
		return (1);

	ret = 0;
	if (!is_valid_env_key(temp_node->key))
	{
		write(2, "minishell: export: `", 20);
		write(2, arg_str, ft_strlen(arg_str));
		write(2, "': not a valid identifier\n", 26);
		ret = 1;
	}
	else
	{
		add_or_update_env(env, temp_node);
	}
	free_envlist(temp_node);
	return (ret);
}

void	ft_export(t_envlist *env, char **arg)
{
	int	i;
	int	has_error;

	has_error = 0;
	i = 1;
	if (!arg[1])
	{
		sort_envlist(env);
		print_export_format(env);
		g_last_exit = 0;
		return ;
	}
	while (arg[i])
	{
		if (handle_single_export_argument(env, arg[i]))
			has_error = 1;
		i++;
	}
	if (has_error)
		g_last_exit = 1;
	else
		g_last_exit = 0;
	sort_envlist(env);
}

void	free_envlist(t_envlist *list)
{
	t_envlist	*current;
	t_envlist	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
