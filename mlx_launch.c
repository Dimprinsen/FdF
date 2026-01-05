/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/05 20:22:29 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	get_scale(int rows, int cols, int z_range)
{
	int		scale;
	double	scale_x;
	double	scale_y;
	double	projected_width;
	double	projected_height;

	projected_width = (cols + rows) * COS_30;
	projected_height = (cols + rows) * SIN_30 + z_range;
	scale_x = (WINDOW_WIDTH * 0.9) / projected_width;
	scale_y = (WINDOW_HEIGHT * 0.9) / projected_height;
	if (scale_x < scale_y)
		scale = (int)scale_x;
	else
		scale = (int)scale_y;
	if (scale < 1)
		scale = 1;
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

static void	setup_and_run(mlx_t *mlx, mlx_image_t *img, t_fdf_data *data)
{
	mlx_image_to_window(mlx, img, 0, 0);
	render_map(img, data->points, data->rows, data->cols);
	mlx_key_hook(mlx, &key_hook, mlx);
	mlx_close_hook(mlx, &close_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_fdf_data	data;

	if (argc != 2)
		return (ft_printf("Usage: ./fdf <map.fdf>\n"), 1);
	data.points = parse_map(argv[1], &data.rows, &data.cols);
	if (!data.points)
		return (1);
	mlx = init_mlx();
	if (!mlx)
		return (free_points(data.points, data.cols, data.rows), 1);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	img = create_image(mlx);
	if (!img)
		return (free_points(data.points, data.cols, data.rows), 1);
	setup_and_run(mlx, img, &data);
	free_points(data.points, data.cols, data.rows);
	return (0);
}
