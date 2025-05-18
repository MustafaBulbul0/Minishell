#ifndef BUILTINS_H
# define BUILTINS_H

# include "utils.h"

typedef struct s_envlist
{
	char				*key;
	char				*value;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_cmd t_cmd;


void	write_line(t_cmd *command);
void	print_location();
void	environment(t_envlist **list, char *envp[]);
void	exit_program(t_cmd *commands);


#endif