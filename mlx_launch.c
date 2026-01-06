/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_launch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/06 20:03:05 by thtinner         ###   ########.fr       */
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

static void	setup_and_run(mlx_t *mlx, mlx_image_t *img, t_map_data *data)
{
	mlx_image_to_window(mlx, img, 0, 0);
	render_map(img, data->points, data->rows, data->cols);
	mlx_key_hook(mlx, &key_hook, mlx);
	mlx_close_hook(mlx, &close_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
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

mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true);
	if (!mlx)
		ft_printf("Error: MLX42 initialization failed\n");
	return (mlx);
}
