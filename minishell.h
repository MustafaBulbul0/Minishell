#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <termcap.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include "./builtins/builtins.h"

typedef enum s_token_type
{
	T_WORD = 0,			// Genel kelime (komut, argüman, dosya adı)
	T_PIPE,				// |
	T_REDIR_OUT,		// >
	T_REDIR_IN,			// <
	T_APPEND,			// >>
	T_HEREDOC,			// <<
	T_AND,				// &&
	T_OR,				// ||
	T_OPEN_PAREN,		// (
	T_CLOSE_PAREN,		// )
	T_NEWLINE,			// \n (komut sonu)
	T_EOF				// End of file/input
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	char					*infile;
	char					*outfile;
	int						append;
	struct s_redirection	*next;
}	t_redirection;

typedef struct	s_cmd
{
	char			*cmd;
	char			**args;
	t_redirection	*redirections;
	struct s_cmd	*next;
}	t_cmd;

int		unclosed_quotes(const char *s);
char	*read_multiline_input(void);
int		ft_strcmp(const char *s1, const char *s2);
t_token	*tokenize(char *input);
char	**smart_split(const char *input);
char	*merge_and_strip_quotes(const char *s);

void	print_commands(t_cmd *cmd_list);
t_cmd	*parse_commands(t_token *tokens);

void	free_commands(t_cmd *cmd_list);
void	free_tokens(t_token *tokens);

#endif