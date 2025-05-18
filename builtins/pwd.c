#include "../minishell.h"

void	print_location(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("minishell: pwd error");
		return (1);
	}
	else
	{
		printf("%s", path);
	}
}
