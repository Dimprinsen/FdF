/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/05 21:51:36 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_2d_point	project_and_scale(t_point *point, int scale)
{
	t_2d_point	projected;

	projected.x = (int)((point->x - point->y) * COS_30 * scale);
	projected.y = (int)((point->x + point->y) * SIN_30 * scale
			- point->z * scale);
	return (projected);
}

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

void	put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	uint8_t	*pixel;
	int		index;

	if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
	{
		index = (y * img->width + x) * sizeof(int32_t);
		pixel = &img->pixels[index];
		*(pixel++) = (uint8_t)(color >> 24);
		*(pixel++) = (uint8_t)(color >> 16);
		*(pixel++) = (uint8_t)(color >> 8);
		*(pixel++) = (uint8_t)(color & 0xFF);
	}
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
		put_pixel(img, p1.x, p1.y, color);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		e2 = err * 2;
		if (e2 > -dy)
			step_x(&p1, p2, &err, dy);
		if (e2 < dx)
			step_y(&p1, p2, &err, dx);
	}
}
