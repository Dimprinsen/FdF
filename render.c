/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/05 20:22:32 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_z_range	get_z_range(t_point ***pts, int rows, int *cols)
{
	t_z_range	range;
	int			i;
	int			j;

	range.min = pts[0][0]->z;
	range.max = pts[0][0]->z;
	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols[i])
		{
			if (pts[i][j]->z < range.min)
				range.min = pts[i][j]->z;
			if (pts[i][j]->z > range.max)
				range.max = pts[i][j]->z;
			j++;
		}
		i++;
	}
	return (range);
}

mlx_t	*init_mlx(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true);
	if (!mlx)
		ft_printf("Error: MLX42 initialization failed\n");
	return (mlx);
}

void	draw_connections(mlx_image_t *img, t_point ***pts, int *p, int *cols)
{
	t_2d_point	p1;
	t_2d_point	p2;

	p1 = project_and_scale(pts[p[0]][p[1]], p[2]);
	p1.x = p1.x + p[3];
	p1.y = p1.y + p[4];
	if (p[1] + 1 < cols[p[0]])
	{
		p2 = project_and_scale(pts[p[0]][p[1] + 1], p[2]);
		p2.x = p2.x + p[3];
		p2.y = p2.y + p[4];
		draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
	}
	if (p[0] + 1 < p[5] && p[1] < cols[p[0] + 1])
	{
		p2 = project_and_scale(pts[p[0] + 1][p[1]], p[2]);
		p2.x = p2.x + p[3];
		p2.y = p2.y + p[4];
		draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
	}
}

static void	init_render_params(int *params, t_point ***pts, int rows, int *cols)
{
	int			max_cols;
	t_z_range	z_range;
	double		map_width;
	double		map_height;

	max_cols = find_max_cols(cols, rows);
	z_range = get_z_range(pts, rows, cols);
	params[2] = get_scale(rows, max_cols, z_range.max - z_range.min);
	map_width = (max_cols - 1 + rows - 1) * COS_30 * params[2];
	map_height = (max_cols - 1 + rows - 1) * SIN_30 * params[2]
		+ (z_range.max - z_range.min) * params[2];
	params[3] = (WINDOW_WIDTH - map_width) / 2
		+ (rows - 1) * COS_30 * params[2];
	params[4] = (WINDOW_HEIGHT - map_height) / 2
		+ z_range.max * params[2];
	params[5] = rows;
}

void	render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
	int	row;
	int	col;
	int	params[6];

	clear_image(img);
	if (rows == 0 || !cols)
		return ;
	init_render_params(params, points, rows, cols);
	row = 0;
	while (row < rows)
	{
		col = 0;
		while (col < cols[row])
		{
			params[0] = row;
			params[1] = col;
			draw_connections(img, points, params, cols);
			col++;
		}
		row++;
	}
}
