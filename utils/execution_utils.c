#include "../minishell.h"

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
	i = -1;
	tmp = env;
	while (tmp)
	{
		arr[++i] = create_env_string(tmp);
		if (!arr[i])
		{
			while (i > 0)
				free(arr[--i]);
			free(arr);
			return (NULL);
		}
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}
