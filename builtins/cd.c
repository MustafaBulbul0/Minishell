#include "../minishell.h"

int	builtin_cd(char **args)
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
	if (!args[1] || ft_strcmp(args[1], "") == 0 || ft_strcmp(args[1], "~") == 0)
	{
		target = getenv("HOME");
		if (!target)
		{
			write(2, "minishell: cd: HOME not set\n", 29);
			free(oldpwd);
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = getenv("OLDPWD");
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

	setenv("OLDPWD", oldpwd, 1);
	setenv("PWD", cwd, 1);
	free(oldpwd);
	return (0);
}
