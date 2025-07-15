# include "../minishell.h"

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

char	**envlist_to_array(t_envlist *env)
{
	int			count;
	int			i;
	t_envlist	*tmp;
	char		**arr;

	i = 0;
	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			arr[i] = ft_strjoin_three(tmp->key, "=", tmp->value);
		else
			arr[i] = ft_strjoin_three(tmp->key, "=", "");
		if (!arr[i])
			return (NULL);
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}
