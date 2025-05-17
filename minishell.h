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
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

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
t_cmd	*token_separate(char *input);

#endif