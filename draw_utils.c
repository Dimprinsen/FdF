/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/08/25 20:55:41 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	step_x(t_2d_point *p1, t_2d_point p2, int *err, int dy)
{
	*err -= dy;
	if (p1->x < p2.x)
		p1->x++;
	else
		p1->x--;
}

void	step_y(t_2d_point *p1, t_2d_point p2, int *err, int dx)
{
	*err += dx;
	if (p1->y < p2.y)
		p1->y++;
	else
		p1->y--;
}

void	draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2,
	uint32_t color)
{
	int	dx;
	int	dy;
	int	err;
	int	e2;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	err = dx - dy;
	while (1)
	{
		if (p1.x >= 0 && p1.x < (int)img->width
			&& p1.y >= 0 && p1.y < (int)img->height)
			mlx_put_pixel(img, p1.x, p1.y, color);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		e2 = err * 2;
		if (e2 > -dy)
			step_x(&p1, p2, &err, dy);
		if (e2 < dx)
			step_y(&p1, p2, &err, dx);
	}
}

void	clear_image(mlx_image_t *img)
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, 0x000000FF);
			x++;
		}
		y++;
	}
}
