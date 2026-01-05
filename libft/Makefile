# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/10 18:35:57 by ttinnerh          #+#    #+#              #
#    Updated: 2026/01/05 21:51:35 by thtinner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a
FILES := ft_split.c ft_atoi.c
OBJS := $(FILES:%.c=%.o)
HEADER := libft.h
CFLAGS := -Wall -Werror -Wextra
ARFLAGS := rcs
AR := ar

.PHONY: all clean fclean re

$(NAME): $(OBJS) $(HEADER)
		$(AR) $(ARFLAGS) $(NAME) $(OBJS)

all: $(NAME)

clean:	
		@echo "Removing object files"
		$(RM) $(OBJS)

fclean: clean
		@echo "Removing the library"
		$(RM) $(NAME)

re: fclean all
	@echo "Removing and recreating everything"
