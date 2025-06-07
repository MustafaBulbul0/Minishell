#include "../minishell.h"

char	*get_env_value(t_envlist *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	update_env_value(t_envlist *env, const char *key, const char *value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return;
		}
		env = env->next;
	}
}


int	builtin_cd(char **args, t_envlist *env)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*target;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		return (1);

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			write(2, "minishell: cd: HOME not set\n", 29);
			free(oldpwd);
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (!target)
		{
			write(2, "minishell: cd: OLDPWD not set\n", 30);
			free(oldpwd);
			return (1);
		}
		printf("%s\n", target);
	}
	else
		target = args[1];

	if (chdir(target) != 0)
	{
		if (ft_strcmp(args[1], "") != 0)
			perror("minishell: cd");
		free(oldpwd);
		return (1);
	}

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd: getcwd after chdir");
		free(oldpwd);
		return (1);
	}
	update_env_value(env, "OLDPWD", oldpwd);
	update_env_value(env, "PWD", cwd);
	free(oldpwd);
	return (0);
}
