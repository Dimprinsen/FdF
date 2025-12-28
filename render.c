/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2024/08/25 20:55:41 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define COS_30 0.86602540378
#define SIN_30 0.5

t_2d_point	project_and_scale(t_point *point, int scale)
{
	t_2d_point	projected;

	projected.x = (int)((point->x - point->y) * COS_30 * scale);
	projected.y = (int)((point->x + point->y) * SIN_30 * scale
			- point->z * scale);
	return (projected);
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

void	render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
	int	row;
	int	col;
	int	params[6];

	clear_image(img);
	if (rows == 0 || !cols)
		return ;
	params[2] = get_scale(rows, find_max_cols(cols, rows));
	params[3] = WINDOW_WIDTH / 2;
	params[4] = WINDOW_HEIGHT / 2;
	params[5] = rows;
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
