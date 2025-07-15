#include "../minishell.h"

void	ft_envp(t_envlist *env)
{
	t_envlist	*temp;

	temp = env;
	while (temp)
	{
		if (temp->value)
			printf("%s=%s\n", temp->key, temp->value);
		else if (temp->eq)
			printf("%s=\n", temp->key);
		temp = temp->next;
	}
}
