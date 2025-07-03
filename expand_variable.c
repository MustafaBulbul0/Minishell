#include "minishell.h"

static int	is_valid_var_start(char c);
static char	*find_var_name(char *str);
static char	*strjoin_char(char *s, char c);
static char	*find_value(char *str, t_envlist *env);
static char	*strjoin_and_free_first(char *s1, const char *s2);

char	*expand_variable(char *str, t_envlist *env)
{
	int		i;
	char	*var_name;
	char	*value;
	char	*result;
	char	*temp;
	char	*status;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				status = ft_itoa(g_last_exit);
				result = strjoin_and_free_first(result, status);
				free(status);
				i += 2;
			}
			else if (is_valid_var_start(str[i + 1]))
			{
				var_name = find_var_name(&str[i + 1]);
				value = find_value(var_name, env);
				temp = value;
				result = strjoin_and_free_first(result, temp);
				free(temp);
				i += ft_strlen(var_name) + 1;
				free(var_name);
			}
			else
			{
				result = strjoin_char(result, '$');
				i++;
			}
		}
		else
		{
			result = strjoin_char(result, str[i]);
			i++;
		}
	}
	return (result);
}

static char	*find_value(char *str, t_envlist *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, str) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

static char	*find_var_name(char *str)
{
	int		i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}

static int	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static char	*strjoin_char(char *s, char c)
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
