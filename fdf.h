/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by ttinnerh          #+#    #+#             */
/*   Updated: 2024/08/25 20:55:41 by ttinnerh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 1024
# define HEIGHT 640
# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
# define COS_30 0.86602540378
# define SIN_30 0.5
# define SCALE_FACTOR 1.0
# define OFFSET_X (WINDOW_WIDTH / 2)
# define OFFSET_Y (WINDOW_HEIGHT / 2)
# include "libft.h"
# include "MLX42/MLX42.h"
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_point
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	t_point;

void	free_points(t_point ***points, int *cols, int rows);
void	print_points(t_point ***points, int *cols, int rows);
void	free_split(char **split);
void	parse_row(t_point **row_points, char **split, int row, int col_count);
char	**split_line(char *line, int *count);
int		count_rows(char *fdfmap);

t_point	***allocate_points(int rows, int **cols);
t_point	***parse_map(char *fdfmap, int *rows, int **cols);
#endif
