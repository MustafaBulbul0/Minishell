#include "../minishell.h"

static char	*get_env_value(t_envlist *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	handle_cd_home(char **target_out)
{
	char	*val;

	val = getenv("HOME");
	if (!val)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		g_last_exit = 1;
		return (1);
	}
	*target_out = val;
	return (0);
}

static int	handle_cd_oldpwd(t_envlist *env, char **target_out)
{
	char	*val;

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

static int	handle_cd_special_cases(char *arg,
	t_envlist *env, char **target_out)
{
	if (!arg || ft_strcmp(arg, "~") == 0)
		return (handle_cd_home(target_out));
	if (ft_strcmp(arg, "-") == 0)
		return (handle_cd_oldpwd(env, target_out));
	return (-1);
}

int	get_cd_target(char **args, t_envlist *env, char **target_out)
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
