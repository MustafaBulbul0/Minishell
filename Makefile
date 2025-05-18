NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c \
	parse/read_line.c \
	parse/utils.c \
	parse/parse.c \
	parse/tokenize.c \
	free/free.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra

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
