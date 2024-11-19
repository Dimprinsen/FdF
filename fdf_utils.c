/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttinnerh <ttinnerh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:35:20 by ttinnerh          #+#    #+#             */
/*   Updated: 2024/08/25 20:55:41 by ttinnerh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void	print_points(t_point ***points, int *cols, int rows)
{
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols[i])
		{
			ft_printf("(%d, %d, %d) ", points[i][j]->x, points[i][j]->y, points[i][j]->z);
			j++;
		}
		ft_printf("\n");
		i++;
	}
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
		col++;
	}
}
