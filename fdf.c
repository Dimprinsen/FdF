/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/12/30 18:35:15 by thtinner         ###   ########.fr       */
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

t_point	***allocate_points(int rows, int **cols)
{
	t_point	***points;

	*cols = malloc(rows * sizeof(int));
	if (!*cols)
		return (ft_printf("Failed to allocate memory for cols\n"), NULL);
	points = malloc(rows * sizeof(t_point **));
	if (!points)
		return (free(*cols), ft_printf("Failed to allocate for points\n"), NULL);
	return (points);
}

int	count_rows(char *fdfmap)
{
	int		fd;
	char	*line;
	int		rows;

	fd = open(fdfmap, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Error: Cannot open file %s\n", fdfmap), -1);
	rows = 0;
	line = get_next_line(fd);
	while (line)
	{
		rows++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (rows);
}

void	process_line(t_point ***pts, int **cols, int row, char *line)
{
	char	**split;

	split = split_line(line, &(*cols)[row]);
	pts[row] = malloc((*cols)[row] * sizeof(t_point *));
	if (!pts[row])
		ft_printf("Failed to allocate memory for row %d\n", row);
	parse_row(pts[row], split, row, (*cols)[row]);
	free_split(split);
}

t_point	***parse_map(char *fdfmap, int *rows, int **cols)
{
	t_point	***points;
	int		fd;

	*rows = count_rows(fdfmap);
	if (*rows < 0)
		return (NULL);
	points = allocate_points(*rows, cols);
	if (!points || !*cols)
		return (ft_printf("Error: Memory allocation failed.\n"), NULL);
	fd = open(fdfmap, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Error: Cannot open file %s\n", fdfmap), NULL);
	if (read_map_lines(points, cols, fd, *rows) < 0)
		return (close(fd), NULL);
	close(fd);
	return (points);
}
