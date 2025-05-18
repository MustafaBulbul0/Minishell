#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_envlist
{
	char				*key;
	char				*value;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_cmd t_cmd;


void	write_line(t_cmd *command);
void	print_location();
void	ft_envp(t_envlist *env);
void	exit_program(t_cmd *commands);
int		ft_cd(char **args);
int		builtin_cd(char **envp);
void	ft_unset(t_envlist *list, char *del_key);


#endif