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
		//ft_printf("Freeing row %d\n", i); //DEBUGGER
		while (j < cols[i])
		{
			//ft_printf("Freeing point[%d][%d]\n", i, j); //DEBUGGER
			free(points[i][j]);
			j++;
		}
		//ft_printf("Freeing points[%d]\n", i); //DEBUGGER
		free(points[i]);
		i++;
	}
	//ft_printf("Freeing points array\n"); //DEBUGGER
	free(points);
	//ft_printf("Freeing cols array\n"); //DEBUGGER
	free(cols);
}

/*void	print_points(t_point ***points, int *cols, int rows)
{
	int	i;
	int	j;

	i = 0;
	//ft_printf("Printing points: rows = %d\n", rows); //DEBUGGER
	//for (int i = 0; i < rows; i++) //DEBUGGER
    //	ft_printf("Row %d: cols = %d\n", i, cols[i]); //DEBUGGER
	while (i < rows)
	{
		j = 0;
		while (j < cols[i])
		{
	//		if (points[i][j] == NULL) //DEBUGGER
    //			ft_printf("Null point at row %d, col %d\n", i, j); //DEBUGGER
	//		else //DEBUGGER
	//			ft_printf("(%d, %d, %d) ", points[i][j]->x, points[i][j]->y, points[i][j]->z);
			j++;
		}
		ft_printf("\n");
		i++;
	}
}*/

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
