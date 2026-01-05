# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/10 18:35:57 by ttinnerh          #+#    #+#              #
#    Updated: 2025/05/16 15:20:19 by thtinner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libft.a
FILES := ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
			ft_tolower.c ft_toupper.c ft_memset.c ft_bzero.c ft_memcpy.c \
			ft_memmove.c ft_memcmp.c ft_memchr.c ft_calloc.c ft_strlen.c \
			ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
			ft_strnstr.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c \
			ft_split.c ft_strmapi.c ft_striteri.c ft_atoi.c ft_itoa.c \
			ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
BONUSFILES := ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
				ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
				ft_lstmap.c
OBJS := $(FILES:%.c=%.o)
BONUSOBJS := $(BONUSFILES:%.c=%.o)
HEADER := libft.h
CFLAGS := -Wall -Werror -Wextra
ARFLAGS := rcs
AR := ar

.PHONY: all bonus clean fclean re

$(NAME): $(OBJS) $(BONUSOBJS) $(HEADER)
		$(AR) $(ARFLAGS) $(NAME) $(OBJS)

all: $(NAME)

bonus: $(OBJS) $(BONUSOBJS) $(HEADER)
		$(AR) $(ARFLAGS) $(NAME) $(OBJS) $(BONUSOBJS)

clean:	
		@echo "Removing object files"
		$(RM) $(OBJS) $(BONUSOBJS)

fclean: clean
		@echo "Removing the library"
		$(RM) $(NAME)

re: fclean all
	@echo "Removing and recreating everything"
