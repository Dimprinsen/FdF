/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/01/06 19:59:55 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

char	**split_line(char *line, int *count)
{
	char	**split;
	int		index;

	split = ft_split(line, ' ');
	index = 0;
	*count = 0;
	while (split[index])
	{
		(*count)++;
		index++;
	}
	return (split);
}

void	free_points(t_point ***points, int *cols, int rows)
{
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols[i])
		{
			free(points[i][j]);
			j++;
		}
		free(points[i]);
		i++;
	}
	free(points);
	free(cols);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	parse_row(t_point **row_points, char **split, int row, int col_count)
{
	int	col;

	col = 0;
	while (col < col_count)
	{
		row_points[col] = malloc(sizeof(t_point));
		row_points[col]->x = col;
		row_points[col]->y = row;
		row_points[col]->z = ft_atoi(split[col]);
		row_points[col]->color = parse_color(split[col]);
		col++;
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
			put_pixel(img, x, y, 0x000000FF);
			x++;
		}
		y++;
	}
}
