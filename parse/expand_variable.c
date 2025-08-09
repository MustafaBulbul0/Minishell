/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esir <esir@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:12 by mubulbul          #+#    #+#             */
/*   Updated: 2025/08/09 17:22:16 by esir             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*strjoin_and_free_first(char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	if (s1)
		ft_memcpy(joined, s1, len1);
	if (s2)
		ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	free(s1);
	return (joined);
}

static void	handle_special_var(char **res, int *i)
{
	char	*status;

	status = ft_itoa(g_last_exit);
	*res = strjoin_and_free_first(*res, status);
	free(status);
	*i += 2;
}

static void	handle_env_var(char **res, char *str, int *i, t_envlist *env)
{
	char	*var_name;
	char	*value;

	var_name = find_var_name(&str[*i + 1]);
	value = find_value(var_name, env);
	*res = strjoin_and_free_first(*res, value);
	free(value);
	*i += ft_strlen(var_name) + 1;
	free(var_name);
}

char	*expand_variable(char *str, t_envlist *env)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				handle_special_var(&result, &i);
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
				handle_env_var(&result, str, &i, env);
			else
				result = strjoin_char(result, str[i++]);
		}
		else
			result = strjoin_char(result, str[i++]);
	}
	return (result);
}

char	*expand_tilde(char *str, t_envlist *env)
{
	char	*home_path;
	char	*expanded_path;

	if (str[0] != '~')
		return (ft_strdup(str));
	home_path = find_value("HOME", env);
	if (!home_path)
	{
		if (ft_strcmp(str, "~") == 0)
			return (ft_strdup("~"));
		else
			return (ft_strdup(str));
	}
	if (ft_strcmp(str, "~") == 0)
	{
		expanded_path = ft_strdup(home_path);
		free(home_path);
		return (expanded_path);
	}
	expanded_path = ft_strjoin(home_path, str + 1);
	free(home_path);
	return (expanded_path);
}
