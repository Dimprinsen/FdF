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

/*
1. Read Map. CHECK
2. Convert map into coordinates and store in a struct array. CHECK
4. Isometric wizardry. (put coordinates up/put pixels) // Place the dots.
5. Draw lines. MiniLibX/MLX42 (Connect the dots) (For this algorithm is needed. DDA is supposedly the simplest. Shenya used Bresenham)
6. Hook for escaping (Confused about this one. Shenyas master plan. For escape.)*/


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
	//if (!*cols)											//DEBUGGER
    //ft_printf("Failed to allocate memory for cols\n"); //DEBUGGER
	points = malloc(rows * sizeof(t_point **));
	//if (!points)										//DEBUGGER
    //ft_printf("Failed to allocate memory for points\n"); //DEBUGGER
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
		//ft_printf(" %s", line); // Print each line to ensure it's read correctly.DEBUGGER
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
	//ft_printf("Number of rows: %d\n", *rows); //DEBUGGER
	points = allocate_points(*rows, cols);
	//if (!points || !*cols) //DEBUGGER
	//{
    //	ft_printf("Error: Memory allocation for points or cols failed.\n"); //DEBUGGER
    //	return (NULL); //DEBUGGER
	//}
	//ft_printf("Memory allocated for points and cols.\n"); //DEBUGGER
	fd = open(fdfmap, O_RDONLY);
	row = 0;
	line = get_next_line(fd);
	while (line)
	{
	//	ft_printf("Parsing line: %s", line); //DEBUGGER
		split = split_line(line, &(*cols)[row]);
	//	ft_printf("Row %d: Col count = %d\n", row, (*cols)[row]); //DEBUGGER
		points[row] = malloc((*cols)[row] * sizeof(t_point *));
	//	if (!points[row]) //DEBUGGER
    //		ft_printf("Failed to allocate memory for row %d\n", row); //DEBUGGER
		parse_row(points[row], split, row, (*cols)[row]);
	//	ft_printf("Row %d parsed successfully.\n", row);
		//for (int i = 0; i < (*cols)[row]; i++) //DEBUGGER
    	//	ft_printf("Parsed point: (%d, %d, %d)\n",  //DEBUGGER
        //      points[row][i]->x, points[row][i]->y, points[row][i]->z); //DEBUGGER
		free_split(split);
		free(line);
		row++;
		line = get_next_line(fd);
	}
	//if (row != *rows) //DEBUGGER
	//{
    //	ft_printf("Error: Expected rows = %d, but only parsed %d rows.\n", *rows, row);
    //	return (NULL);
	//}
	ft_printf("parse_map completed successfully.\n");
	return points;
}
/*
int	main(int argc, char *argv[])
{
	int		rows;
	int		*cols;
	t_point	***points;

	if (argc != 2)
	{
		ft_printf("Incorrect. Proper usage is as follows: ./fdf map.fdf\n");
		return (1);
	}
	ft_printf("Map file: %s\n", argv[1]); //DEBUGGER
	ft_printf("Parsing map...\n"); //DEBUGGER
	points = parse_map(argv[1], &rows, &cols);
	if (!points)
	{
		ft_printf("Error: parse_map returned NULL.\n"); //DEBUGGER
		return (1);
	}
	ft_printf("Map parsed successfully. Rows: %d\n", rows); //DEBUGGER
	print_points(points, cols, rows);
	free_points(points, cols, rows);
	return (0);
}*/

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
