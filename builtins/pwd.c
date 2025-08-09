/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:15:32 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 12:15:33 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_pwd(void)
{
	char	*path;
	int		status;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("minishell: pwd error");
		status = 1;
	}
	else
	{
		printf("%s\n", path);
		free(path);
		status = 0;
	}
	g_last_exit = status;
}
