#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <term.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include "./builtins/builtins.h"
# include "./execute/execution.h"
# include "./parse/parse.h"

extern int	g_last_exit;

typedef enum s_token_type
{
	T_WORD = 0,
	T_PIPE,
	T_REDIR_OUT,
	T_REDIR_IN,
	T_APPEND,
	T_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	int				quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	t_token_type			type;
	char					*infile;
	char					*outfile;
	int						append;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redirection	*redirections;
	struct s_cmd	*next;
}	t_cmd;

char		*read_multiline_input(void);
t_token		*tokenize(char *input);
char		**smart_split(const char *input);
char		*merge_and_strip_quotes(const char *s);
void		print_commands(t_cmd *cmd_list);
t_cmd		*parse_commands(t_token *tokens);
void		ft_execute(t_envlist *env, t_cmd *command);
void		free_commands(t_cmd *cmd_list);
void		free_tokens(t_token *tokens);
void		free_env(t_envlist *env);
t_envlist	*envp_init(char *envp[]);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strndup(const char *s, size_t n);
int			is_numeric(char *str);
char		**ft_strdup_two_dimension(char **arg);
void		ft_free_split(char	**arg);
char		*ft_strjoin_three(char *s1, char *s2, char *s3);
char		**envlist_to_array(t_envlist *env);
char		*expand_variable(char *str, t_envlist *env);
int			is_builtin(t_cmd *cmd);
char		*get_exec_path(char *cmd);
t_token		*new_token(char *str, t_token_type type, int quote_type);
char		*strjoin_char(char *s, char c);
char		*find_var_name(char *str);
char		*find_value(char *str, t_envlist *env);
void		parse_execute(char *input, t_envlist *env);
void		execute_builtin(t_cmd *cmd, t_envlist *env, int is_child);
char		*expand_tilde(char *str, t_envlist *env);

#endif