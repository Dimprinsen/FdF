/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/08/25 20:55:41 by thtinner         ###   ########.fr       */
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
	ft_printf("Printing points: rows = %d\n", rows);
	while (i < rows)
	{
		ft_printf("Row %d: cols = %d\n", i, cols[i]);
		j = 0;
		while (j < cols[i])
		{
			if (points[i][j] == NULL)
				ft_printf("Null point at row %d, col %d\n", i, j);
			else
				ft_printf("(%d, %d, %d) ", points[i][j]->x,
					points[i][j]->y, points[i][j]->z);
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
		row_points[col]->color = parse_color(split[col],
				row_points[col]->z);
		col++;
	}
}

int	read_map_lines(t_point ***pts, int **cols, int fd, int rows)
{
	char	*line;
	int		row;

	row = 0;
	line = get_next_line(fd);
	while (line)
	{
		process_line(pts, cols, row, line);
		free(line);
		row++;
		line = get_next_line(fd);
	}
	close(fd);
	if (row != rows)
		return (ft_printf("Error: rows mismatch.\n"), -1);
	return (0);
}
