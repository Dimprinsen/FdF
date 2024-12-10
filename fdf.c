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

t_point ***allocate_points(t_env *env)
{
    t_point ***points;

    points = malloc(env->rows * sizeof(t_point **));
    env->cols = malloc(env->rows * sizeof(int));
    return points;
}

int count_rows(char *fdfmap)
{
    int fd;
    char *line;
    int rows;

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

t_point ***parse_map(char *fdfmap, t_env *env)
{
    t_point ***points;
    int fd;
    char *line;
    int row;
    char **split;

    env->rows = count_rows(fdfmap); // Use env->rows
    points = allocate_points(env);  // Pass env directly
    fd = open(fdfmap, O_RDONLY);
    row = 0;
    line = get_next_line(fd);
    while (line)
    {
        split = split_line(line, &env->cols[row]);
        points[row] = malloc(env->cols[row] * sizeof(t_point *));
        parse_row(points[row], split, row, env->cols[row]);
        free_split(split);
        free(line);
        row++;
        line = get_next_line(fd);
    }
    ft_printf("parse_map completed successfully.\n");
    return points;
}

