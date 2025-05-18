#include "../minishell.h"

void	ft_unset(t_envlist *list, char *del_key)
{
	t_envlist	*temp;

	temp = list;
	if (ft_strcmp(temp->key, del_key) == 0)
		list = list->next;
	else
	{
		while (temp->next)
		{
			if (ft_strcmp(temp->next->key, del_key) == 0)
			{
				temp->next = temp->next->next;
				break ;
			}
			temp = temp->next;
		}
	}
}
