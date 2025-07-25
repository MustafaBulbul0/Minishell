#include "../minishell.h"

void	execute_builtin(t_cmd *cmd, t_envlist *env, int is_child)
{
	if (!cmd || !cmd->cmd)
		return ;
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		builtin_env(env);
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		builtin_exit(cmd, is_child);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		builtin_cd(cmd->args, env);
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		builtin_unset(env, cmd->args);
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		builtin_export(env, cmd->args);
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
