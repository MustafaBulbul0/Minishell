#include "./../minishell.h"

static char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

static void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	ft_access(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (0);
	path_env = getenv("PATH");
	if (!path_env)
		return (1);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (1);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			free(full_path);
			ft_free_split(paths);
			return (0);
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (1);
}
