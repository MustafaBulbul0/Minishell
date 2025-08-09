/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:17:55 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 15:36:51 by mubulbul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*find_var_name(char *str)
{
	int		i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

char	*find_value(char *str, t_envlist *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, str) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*strjoin_char(char *s, char c)
{
	size_t	len;
	char	*new_str;

	if (!s)
	{
		new_str = (char *)malloc(2);
		if (!new_str)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = strlen(s);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	memcpy(new_str, s, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(s);
	return (new_str);
}

int	get_last_process_status(pid_t last_pid)
{
	int	status;
	int	exit_status;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit_status = 130;
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit: 3\n", 8);
			exit_status = 131;
		}
		else
			exit_status = 128 + WTERMSIG(status);
	}
	else
		exit_status = status;
	return (exit_status);
}
