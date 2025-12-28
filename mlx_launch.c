/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2024/08/25 20:55:41 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	get_scale(int rows, int cols)
{
	int	scale_x;
	int	scale_y;
	int	scale;

	scale_x = (WINDOW_WIDTH * 0.7) / (cols + rows / 2);
	scale_y = (WINDOW_HEIGHT * 0.7) / (rows + cols / 2);
	if (scale_x < scale_y)
		scale = scale_x;
	else
		scale = scale_y;
	if (scale < 1)
		scale = 1;
	if (scale > 30)
		scale = 30;
	return (scale);
}

int	find_max_cols(int *cols, int rows)
{
	int	i;
	int	max;

	i = 0;
	max = 0;
	while (i < rows)
	{
		if (cols[i] > max)
			max = cols[i];
		i++;
	}
	return (max);
}

static mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_printf("Error: MLX42 initialization failed\n");
	return (mlx);
}

static mlx_image_t	*create_image(mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img)
	{
		ft_printf("Error: Failed to create image\n");
		mlx_terminate(mlx);
	}
	return (img);
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_point		***points;
	int			rows;
	int			*cols;

	if (argc != 2)
		return (ft_printf("Usage: ./fdf <map.fdf>\n"), 1);
	points = parse_map(argv[1], &rows, &cols);
	if (!points)
		return (1);
	mlx = init_mlx();
	if (!mlx || (mlx_set_setting(MLX_STRETCH_IMAGE, true), 0))
		return (1);
	img = create_image(mlx);
	if (!img)
		return (1);
	mlx_image_to_window(mlx, img, 0, 0);
	render_map(img, points, rows, cols);
	mlx_key_hook(mlx, &key_hook, mlx);
	mlx_close_hook(mlx, &close_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_points(points, cols, rows);
	return (0);
}
