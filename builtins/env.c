#include "../minishell.h"

t_envlist	*create_env_node(char *env_string);
void	add_node(t_envlist **list, t_envlist *new_node);
int	print_env(t_envlist *node);

void	environment(t_envlist **list, char *envp[])
{
	t_envlist	*new_node;
	int			i;

	if (!list)
		return ;
	*list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
			return ;
		add_node(list, new_node);
		i++;
	}
	print_env(*list);
}

t_envlist	*create_env_node(char *env_string)
{
	t_envlist	*node;
	char		*key;
	char		*value;
	int			i;

	i = 0;
	while (env_string[i] != '=')
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
	node->next = NULL;
	return (node);
}

void	add_node(t_envlist **list, t_envlist *new_node)
{
	t_envlist	*current;

	if (!list || !new_node)
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

int	print_env(t_envlist *node)
{
	while (node)
	{
		printf("%s", node->key);
		if (node->value)
		{
			printf("=");
			printf("%s", node->value);
		}
		printf("\n");
		node = node->next;
	}
	return (0);
}
