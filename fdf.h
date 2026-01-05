/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by ttinnerh          #+#    #+#             */
/*   Updated: 2026/01/05 20:31:13 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 640
# define COS_30 0.86602540378
# define SIN_30 0.5
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"
# include "MLX42/MLX42.h"
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>

typedef struct s_point
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	t_point;

typedef struct s_2d_point
{
	int	x;
	int	y;
}	t_2d_point;

typedef struct s_z_range
{
	int	min;
	int	max;
}	t_z_range;

typedef struct s_fdf_data
{
	t_point	***points;
	int		rows;
	int		*cols;
}	t_fdf_data;

void		free_points(t_point ***points, int *cols, int rows);
void		free_split(char **split);
void		parse_row(t_point **row_points, char **split, int row,
				int col_count);
int			read_map_lines(t_point ***pts, int **cols, int fd, int rows);
char		**split_line(char *line, int *count);
int			count_rows(char *fdfmap);
t_point		***allocate_points(int rows, int **cols);
t_point		***parse_map(char *fdfmap, int *rows, int **cols);
void		process_line(t_point ***pts, int **cols, int row, char *line);
void		render_map(mlx_image_t *img, t_point ***points, int rows,
				int *cols);
mlx_t		*init_mlx(void);
void		draw_connections(mlx_image_t *img, t_point ***pts, int *p,
				int *cols);
t_2d_point	project_and_scale(t_point *point, int scale);
void		draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2,
				uint32_t c);
void		step_x(t_2d_point *p1, t_2d_point p2, int *err, int dy);
void		step_y(t_2d_point *p1, t_2d_point p2, int *err, int dx);
void		put_pixel(mlx_image_t *img, int x, int y, uint32_t color);
void		clear_image(mlx_image_t *img);
void		key_hook(mlx_key_data_t keydata, void *param);
void		close_hook(void *param);
uint32_t	parse_color(char *str);
int			get_scale(int rows, int cols, int z_range);
int			find_max_cols(int *cols, int rows);

#endif
