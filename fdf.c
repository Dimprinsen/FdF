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

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

void	free_points(t_point ***points, int *cols, int rows);
void	print_points(t_point ***points, int *cols, int rows);

/*1. Read Map. CHECK
2. Convert map into coordinates and store in a struct array.
4. Isometric wizardry. (put coordinates up/put pixels) // Place the dots.
5. Draw lines. MiniLibX/MLX42 (Connect the dots) (For this algorithm is needed. DDA is supposedly the simplest. Shenya used Bresenham)
6. Hook for escaping (Confused about this one. Shenyas master plan. For escape.)

/*
int	count_column(char *line)
{
	char	**split;
	int		count;
	int		i;

	split = ft_split(line, ' ');
	Count = 0;
	while (split, [count])
		count++;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);

	return (count);
}

void	read_and_store_file(char *fdfmap)
{
	int	i = 0;
	int	j = 0;
	char	**result = NULL;

	int	fd;
	fd = open(fdfmap, O_RDONLY);
	if (fd < 0)
	{
		printf("Error reading map.");
		return ;
	}
	char	*line;
	while((line = get_next_line(fd)))
	{
		printf("%s", line);
		i++;
		if (i == 1)
		{
			result = ft_split(line);
			j = count_column(result);
		}
		free(line);
	}
	close(fd);
	printf("Lines: %i\n", i);
	printf("Columns: %d\n", j);
}*/
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
	points = malloc(rows * sizeof(t_point **));
	return (points);
}

int	count_rows(char *fdfmap)
{
	int		fd;
	char	*line;
	int		rows;

	fd = open(fdfmap, O_RDONLY);
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

t_point	***parse_map(char *fdfmap, int *rows, int **cols)
{
	t_point	***points;
	int		fd;
	char	*line;
	int		row;
	char	**split;

	*rows = count_rows(fdfmap);
	points = allocate_points(*rows, cols);
	fd = open(fdfmap, O_RDONLY);
	row = 0;
	line = get_next_line(fd);
	while (line)
	{
		split = split_line(line, &(*cols)[row]);
		points[row] = malloc((*cols)[row] * sizeof(t_point *));
		parse_row(points[row], split, row, (*cols)[row]);
		free_split(split);
		free(line);
		row++;
		line = get_next_line(fd);
	}
}

int	main(int argc, char *argv[])
{
	int		rows;
	int		*cols;
	t_point	***points;

	if (argc != 2)
	{
		printf("Incorrect. Proper usage is as follows: ./fdf map.fdf\n");
		return (1);
	}
	points = parse_map(argv[1], &rows, &cols);
	if (!points)
		return (1);
	print_points(points, cols, rows);
	free_points(points, cols, rows);
	return (0);
}

/*

Turn in files: Makefile, *.h, *.c
Makefile: NAME, all, clean, fclean, re
Arguments: A file in format *.fdf

External functs.
• open, close, read, write, malloc, free, perror, strerror, exit
• All functions of the math library (-lm compiler option, man man 3 math)
• All functions of the MiniLibX
• ft_printf and any equivalent YOU coded

For one degree of projection:
destination.x = source.x + cos(angle) * source.z
destination.y = source.y + sin(angle) * source.z

For 120 degrees of projection (for isomertric):
destination.x=source.x * cos(a)+source.y * cos(a+2)+source.z * cos(a - 2)
destination.y=source.x * sin(a)+source.y * sin(a+2)+source.z * sin(a - 2)

*/
