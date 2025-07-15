#include "../minishell.h"

static char	*search_path_in_env(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_exec_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || *cmd == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_path_in_env(paths, cmd);
	ft_free_split(paths);
	return (result);
}
