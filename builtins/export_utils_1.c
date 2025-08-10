/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa <mustafa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:22 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/10 17:20:07 by mustafa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	parse_with_equal(t_envlist *node, const char *s, char *eq_pos)
{
	node->key = ft_substr(s, 0, eq_pos - s);
	node->value = ft_strdup(eq_pos + 1);
	node->eq = 1;
}

void	free_envlist(t_envlist *list)
{
	t_envlist	*current;
	t_envlist	*next;

	current = list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
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
		parse_with_equal(node, s, eq_pos);
	else
	{
		node->key = ft_strdup(s);
		node->value = NULL;
		node->eq = 0;
	}
	if (!node->key || (eq_pos && !node->value))
	{
		free_envlist(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

int	handle_single_export_arg(t_envlist *env, const char *arg_str)
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
		add_or_update_env(env, temp_node);
	free_envlist(temp_node);
	return (ret);
}
