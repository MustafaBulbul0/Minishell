#include "../minishell.h"

void	ft_envp(t_envlist *env)
{
	t_envlist	*temp;

	temp = env;
	while (temp)
	{
		if (temp->value)
			printf("%s%s%s\n", temp->key,"=",temp->value);
		temp = temp->next;
	}
}
