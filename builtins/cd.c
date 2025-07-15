#include "../minishell.h"

static int	perform_chdir(char *target_path, t_envlist *env);

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

static void	update_env_value(t_envlist *env, const char *key, const char *value)
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
