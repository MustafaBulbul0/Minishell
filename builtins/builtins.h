#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_envlist
{
	char				*key;
	char				*value;
	int					eq;
	struct s_envlist	*next;
}	t_envlist;

typedef struct s_cmd	t_cmd;

void	builtin_echo(t_cmd *command);
void	builtin_pwd(void);
void	builtin_env(t_envlist *env);
void	builtin_exit(t_cmd *commands, int is_child);
int		builtin_cd(char **envp, t_envlist *env);
void	builtin_unset(t_envlist *list, char **arg);
void	builtin_export(t_envlist	*env, char **arg);
int		get_cd_target(char **args, t_envlist *env, char **target_out);
int		handle_single_export_arg(t_envlist *env, const char *arg_str);
void	add_or_update_env(t_envlist *env, t_envlist *node);

#endif