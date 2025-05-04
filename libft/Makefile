# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mubulbul <mubulbul@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/19 11:37:20 by mubulbul          #+#    #+#              #
#    Updated: 2024/10/19 11:37:21 by mubulbul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CFLAGS = -Wall -Wextra -Werror
CC = gcc
FILES =	ft_atoi.c \
		ft_memchr.c \
		ft_split.c \
		ft_strncmp.c \
		ft_bzero.c \
		ft_memcmp.c \
		ft_strchr.c \
		ft_strnstr.c \
		ft_calloc.c \
		ft_memcpy.c \
		ft_strdup.c \
		ft_strrchr.c \
		ft_isalnum.c \
		ft_memmove.c \
		ft_striteri.c \
		ft_strtrim.c \
		ft_isalpha.c \
		ft_memset.c \
		ft_strjoin.c \
		ft_substr.c \
		ft_isascii.c \
		ft_putchar_fd.c \
		ft_strlcat.c \
		ft_tolower.c \
		ft_isdigit.c \
		ft_putendl_fd.c \
		ft_strlcpy.c \
		ft_toupper.c \
		ft_isprint.c \
		ft_putnbr_fd.c \
		ft_strlen.c \
		ft_itoa.c \
		ft_putstr_fd.c \
		ft_strmapi.c

BONUS = ft_lstadd_front.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c \
		ft_lstclear.c \
		ft_lstadd_back.c

OBJ = $(FILES:.c=.o)
BONUS_OBJ = $(BONUS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: $(OBJ) $(BONUS_OBJ)
	ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

.PHONY: clean fclean all bonus re
