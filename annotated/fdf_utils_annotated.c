/* ************************************************************************** */
/* Annotated copy of fdf_utils.c — helpers and memory management explained */
/* ************************************************************************** */

#include "fdf.h"

/*
 * split_line
 * - Split a map line into tokens and count columns.
 */
char	**split_line(char *line, int *count)
{
    char	**split;
    int		index;

    split = ft_split(line, ' '); // use libft split to tokenize on spaces
    index = 0;
    *count = 0; // initialize column count
    while (split[index])
    {
        (*count)++; // count tokens
        index++;
    }
    return (split);
}

/*
 * free_points
 * - Free the full 3D points array and the cols array that stores widths.
 */
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
            free(points[i][j]); // free each t_point
            j++;
        }
        free(points[i]); // free row pointer array
        i++;
    }
    free(points); // free top-level pointer
    free(cols);   // free column sizes array
}

/*
 * free_split
 * - Helper to free the array returned by ft_split.
 */
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

/*
 * parse_row
 * - Allocate and fill t_point structs for a single parsed row.
 */
void	parse_row(t_point **row_points, char **split, int row, int col_count)
{
    int	col;

    col = 0;
    while (col < col_count)
    {
        row_points[col] = malloc(sizeof(t_point)); // allocate point
        row_points[col]->x = col; // store column index as x
        row_points[col]->y = row; // store row index as y
        row_points[col]->z = ft_atoi(split[col]); // height from token
        row_points[col]->color = parse_color(split[col]); // parse optional color
        col++;
    }
}

/*
 * clear_image
 * - Fill the entire image with opaque black by calling put_pixel for each
 *   position. This is simple but O(width*height).
 */
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
            put_pixel(img, x, y, 0x000000FF); // black opaque
            x++;
        }
        y++;
    }
}
