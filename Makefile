# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 01:34:27 by thtinner          #+#    #+#              #
#    Updated: 2026/02/15 17:43:41 by thtinner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
MLX_DIR = ./MLX42
LIBFT_DIR = ./libft

# Library files
MLX42 = $(MLX_DIR)/build/libmlx42.a
LIBFT = libft.a

# Sources and objects
SRCS = fdf.c fdf_utils.c mlx_launch.c render.c draw_utils.c color_utils.c hooks.c
OBJS = $(SRCS:.c=.o)

# Includes
INCLUDES = -I$(LIBFT_DIR) -I$(MLX_DIR)/include

# MLX42 linker flags
MLX_FLAGS = -ldl -lglfw -pthread -lm

# Output executable
NAME = fdf

# Rules
all: $(LIBFT) $(MLX42) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@cp $(LIBFT_DIR)/libft.a .

$(MLX42):
	@if [ ! -f $(MLX42) ]; then \
		echo "Error: MLX42 not built. Run: cd MLX42 && cmake -B build && cmake --build build"; \
		exit 1; \
	fi

$(NAME): $(OBJS) $(LIBFT) $(MLX42)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX42) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(LIBFT)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

