NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJDIR = OBJ

SRCS = main.c \
	parse/read_line.c parse/parse.c parse/tokenize.c parse/tokenize_utils.c \
	parse/fill_command.c parse/process_token.c parse/parse_execute.c \
	parse/syntax_error.c parse/expand_variable.c \
	execute/execution.c execute/execute_pipe.c execute/handle_heredoc.c \
	execute/external_command.c execute/handle_redirection.c builtins/cd_utils.c \
	builtins/cd.c builtins/echo.c builtins/export_utils_1.c builtins/export_utils_2.c \
	builtins/pwd.c builtins/unset.c builtins/env.c builtins/exit.c builtins/export.c \
	utils/envp_init.c utils/access_path.c utils/execution_utils.c \
	utils/expand_utils.c utils/utils.c utils/parse_utils.c utils/free.c

OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	make -C $(LIBFT_DIR) clean
	$(RM) -r $(OBJDIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

leaks: all
	@valgrind --leak-check=full							\
			  --suppressions=./readline.supp			\
			  --show-leak-kinds=all						\
			  --track-origins=yes						\
			  --track-fds=yes							\
			  --verbose									\
			  ./$(NAME)