NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c \
	expand_variable.c \
	parse/read_line.c parse/parse.c parse/tokenize.c parse/tokenize_utils.c \
	parse/fill_command.c parse/process_token.c parse/parse_execute.c \
	execute/execution.c execute/execute_pipe.c \
	execute/external_command.c execute/handle_redirection.c \
	builtins/cd.c builtins/echo.c builtins/exit.c builtins/export.c builtins/export_utils_1.c builtins/export_utils_2.c \
	builtins/pwd.c builtins/unset.c builtins/env.c \
	builtins/cd_utils.c \
	utils/envp_init.c utils/access_path.c utils/execution_utils.c utils/utils.c utils/parse_utils.c \
	utils/expand_utils.c free/free.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all
