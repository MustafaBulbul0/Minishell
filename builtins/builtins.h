#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_envlist
{
	char				*key;
	char				*value;
	int					eq;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_cmd t_cmd;


void	write_line(t_cmd *command);
void	print_location(void);
void	ft_envp(t_envlist *env);
void	exit_program(t_cmd *commands);
int		builtin_cd(char **envp, t_envlist *env);
void	ft_unset(t_envlist *list, char **arg);
void	ft_export(t_envlist	*env, char **arg);
void	sort_envlist(t_envlist *list);
void	print_export_format(t_envlist *list);


#endif