/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 00:52:43 by ttinnerh          #+#    #+#             */
/*   Updated: 2024/11/28 00:52:50 by ttinnerh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
	if (!img || !points || rows <= 0 || !cols)
	{
		fprintf(stderr, "Invalid input to render_map\n");
		return ;
	}
	double	scale_factor = calculate_scale_factor(rows, cols, points, WINDOW_WIDTH, WINDOW_HEIGHT);
	double	min_x, max_x, min_y, max_y;

	if (calculate_isometric_bounds(points, rows, cols, &min_x, &max_x, &min_y, &max_y, scale_factor) != 0)
	{
		fprintf(stderr, "Failed to calculate isometric bounds\n");
		return ;
	}
    int offset_x = OFFSET_X - (int)((max_x + min_x) / 2);
    int offset_y = OFFSET_Y - (int)((max_y + min_y) / 2);
    t_2d_point p1, p2;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols[row]; col++) {
            p1 = isometric_projection(points[row][col], scale_factor, offset_x, offset_y);
            if (col + 1 < cols[row]) {
                p2 = isometric_projection(points[row][col + 1], scale_factor, offset_x, offset_y);
                draw_l(img, p1, p2, 0xFFFFFF);
            }
            if (row + 1 < rows && col < cols[row + 1]) {
                p2 = isometric_projection(points[row + 1][col], scale_factor, offset_x, offset_y);
                draw_l(img, p1, p2, 0xFFFFFF);
			}
		}
	}
}

void	draw_l(mlx_image_t *img, t_2d_point p1, t_2d_point p2, uint32_t color)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

	while (1)
    {
        mlx_put_pixel(img, p1.x, p1.y, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            p1.x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            p1.y += sy;
        }
    }
}

void	error_callback(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
}

static void	remove_exit(mlx_t *mlx, t_point ***points, int *cols, int rows)
{
	mlx_terminate(mlx);
	free_points(points, cols, rows);
	exit(0);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t		*mlx;
	static int	is_terminating;

	mlx = (mlx_t *)param;
	is_terminating = 0;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		if (!is_terminating)
		{
			is_terminating = 1;
			remove_exit(mlx, NULL, NULL, 0);
		}
	}
}

mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
	{
		error_callback("MLX42 initialization failed");
		return (NULL);
	}
	return (mlx);
}

mlx_image_t	*create_image(mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		return (NULL);
	}
	return (img);
}

t_point	***parse_map_and_check(int argc, char **argv, int *rows, int **cols)
{
	t_point	***points;

	points = parse_map(argv[1], rows, cols);
	if (argc != 2)
	{
		ft_printf("Usage: ./fdf <map.fdf>\n");
		return (NULL);
	}
	if (!points)
	{
		ft_printf("Failed to parse map\n");
		return (NULL);
	}
	return (points);
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_point		***points;
	int			rows;
	int			*cols;

	points = parse_map_and_check(argc, argv, &rows, &cols);
	if (!points)
		return (1);
	mlx = init_mlx();
	if (!mlx)
		return (1);
	img = create_image(mlx);
	if (!img)
		return (1);
	mlx_image_to_window(mlx, img, 0, 0);
	render_map(img, points, rows, cols);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);
	free_points(points, cols, rows);
	return (0);
}
