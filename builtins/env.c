/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:15 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:15:16 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_env(t_envlist *env)
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
