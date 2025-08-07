#include "./../minishell.h"

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
