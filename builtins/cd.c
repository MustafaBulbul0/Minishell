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
			return ;
		}
		env = env->next;
	}
}

static int	update_cd_env_vars(t_envlist *env, char *oldpwd_val)
{
	char	new_cwd[1024];

	if (!getcwd(new_cwd, sizeof(new_cwd)))
	{
		perror("minishell: cd: getcwd after chdir");
		g_last_exit = 1;
		return (1);
	}
	update_env_value(env, "OLDPWD", oldpwd_val);
	update_env_value(env, "PWD", new_cwd);
	return (0);
}

static int	handle_cd_special_cases(char *arg,
		t_envlist *env, char **target_out)
{
	char	*val;

	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		val = get_env_value(env, "HOME");
		if (!val)
		{
			write(2, "minishell: cd: HOME not set\n", 29);
			g_last_exit = 1;
			return (1);
		}
		*target_out = val;
		return (0);
	}
	if (ft_strcmp(arg, "-") == 0)
	{
		val = get_env_value(env, "OLDPWD");
		if (!val)
		{
			write(2, "minishell: cd: OLDPWD not set\n", 30);
			g_last_exit = 1;
			return (1);
		}
		printf("%s\n", val);
		*target_out = val;
		return (0);
	}
	return (-1);
}

static int	get_cd_target(char **args, t_envlist *env, char **target_out)
{
	int	argc;
	int	special_case_status;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		g_last_exit = 1;
		return (1);
	}
	special_case_status = handle_cd_special_cases(args[1], env, target_out);
	if (special_case_status != -1)
		return (special_case_status);
	if (args[1] && ft_strcmp(args[1], "") == 0)
	{
		g_last_exit = 0;
		return (2);
	}
	*target_out = args[1];
	return (0);
}

static int	perform_chdir(char *target_path, t_envlist *env)
{
	char	*oldpwd_val;
	int		status;

	oldpwd_val = getcwd(NULL, 0);
	if (!oldpwd_val)
	{
		perror("minishell: cd: getcwd");
		g_last_exit = 1;
		return (1);
	}
	if (chdir(target_path) != 0)
	{
		perror("minishell: cd");
		free(oldpwd_val);
		g_last_exit = 1;
		return (1);
	}
	status = update_cd_env_vars(env, oldpwd_val);
	free(oldpwd_val);
	g_last_exit = status;
	return (status);
}

int	builtin_cd(char **args, t_envlist *env)
{
	char	*target_path;
	int		status;

	status = get_cd_target(args, env, &target_path);
	if (status == 1)
		return (1);
	if (status == 2)
		return (0);
	return (perform_chdir(target_path, env));
}
