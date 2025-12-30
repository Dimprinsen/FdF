# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/21 01:34:27 by thtinner          #+#    #+#              #
#    Updated: 2025/12/30 21:10:59 by thtinner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
MLX_DIR = ./MLX42
FT_PRINTF_DIR = ./ft_printf
GNL_DIR = ./get_next_line
LIBFT_DIR = ./libft

# Library files
MLX42 = $(MLX_DIR)/build/libmlx42.a
LIBFT = libft.a

# Sources and objects
SRCS = fdf.c fdf_utils.c mlx_launch.c render.c draw_utils.c color_utils.c hooks.c
OBJS = $(SRCS:.c=.o)

# ft_printf sources
FT_PRINTF_SRCS = $(FT_PRINTF_DIR)/ft_printf.c $(FT_PRINTF_DIR)/ft_printf_util.c
FT_PRINTF_OBJS = $(FT_PRINTF_SRCS:.c=.o)

# get_next_line sources
GNL_SRCS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
GNL_OBJS = $(GNL_SRCS:.c=.o)

# Includes
INCLUDES = -I$(LIBFT_DIR) -I$(MLX_DIR)/include -I$(FT_PRINTF_DIR) -I$(GNL_DIR)

# MLX42 linker flags
MLX_FLAGS = -ldl -lglfw -pthread -lm

# Output executable
NAME = fdf

# Rules
all: mlx_check $(NAME)

mlx_check:
	@if [ ! -f "$(MLX42)" ]; then \
		echo "Error: MLX42 not found. Please install it:"; \
		echo "  git clone https://github.com/codam-coding-college/MLX42.git"; \
		echo "  cd MLX42 && cmake -B build && cmake --build build"; \
		exit 1; \
	fi

$(NAME): $(OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS) $(LIBFT) $(MLX42) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re mlx_check

