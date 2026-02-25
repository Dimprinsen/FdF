/* ************************************************************************** */
/* Annotated copy of render.c — line-by-line comments explaining rendering */
/* ************************************************************************** */

#include "fdf.h"

/*
 * get_z_range
 * - Walk the entire point grid to find min and max Z values (elevation).
 */
static t_z_range	get_z_range(t_point ***pts, int rows, int *cols)
{
    t_z_range	range;
    int			i;
    int			j;

    // Initialize min/max from the first point
    range.min = pts[0][0]->z;
    range.max = pts[0][0]->z;
    i = 0;
    while (i < rows)
    {
        j = 0;
        while (j < cols[i])
        {
            if (pts[i][j]->z < range.min)
                range.min = pts[i][j]->z; // update minimum
            if (pts[i][j]->z > range.max)
                range.max = pts[i][j]->z; // update maximum
            j++;
        }
        i++;
    }
    return (range);
}

/*
 * draw_connections
 * - Project the point at (p[0], p[1]) then connect to the right and down
 *   neighbor if present. p array contains rendering parameters.
 */
void	draw_connections(mlx_image_t *img, t_point ***pts, int *p, int *cols)
{
    t_2d_point	p1;
    t_2d_point	p2;

    // Project and scale current point to 2D
    p1 = project_and_scale(pts[p[0]][p[1]], p[2]);
    // Apply computed offsets to position the map inside the window
    p1.x = p1.x + p[3];
    p1.y = p1.y + p[4];
    if (p[1] + 1 < cols[p[0]])
    {
        // If there is a right neighbor, project and draw line to it
        p2 = project_and_scale(pts[p[0]][p[1] + 1], p[2]);
        p2.x = p2.x + p[3];
        p2.y = p2.y + p[4];
        draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
    }
    if (p[0] + 1 < p[5] && p[1] < cols[p[0] + 1])
    {
        // If there is a below neighbor, project and draw line downwards
        p2 = project_and_scale(pts[p[0] + 1][p[1]], p[2]);
        p2.x = p2.x + p[3];
        p2.y = p2.y + p[4];
        draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
    }
}

/*
 * init_render_params
 * - Compute scale and offsets to center/fit the projected map in the window.
 */
static void	init_render_params(int *params, t_point ***pts, int rows, int *cols)
{
    int			max_cols;
    t_z_range	z_range;
    double		map_width;
    double		map_height;

    // Largest row length to compute bounding box
    max_cols = find_max_cols(cols, rows);
    // Vertical elevation range across the map
    z_range = get_z_range(pts, rows, cols);
    // Choose an integer scale so the whole map fits the window
    params[2] = get_scale(rows, max_cols, z_range.max - z_range.min);
    // Approximate projected pixel width of the whole map
    map_width = (max_cols - 1 + rows - 1) * COS_30 * params[2];
    // Approximate projected pixel height (includes z-range effect)
    map_height = (max_cols - 1 + rows - 1) * SIN_30 * params[2]
        + (z_range.max - z_range.min) * params[2];
    // X offset: center horizontally and adjust for isometric anchor
    params[3] = (WINDOW_WIDTH - map_width) / 2
        + (rows - 1) * COS_30 * params[2];
    // Y offset: center vertically and lift by max z so tall points don't clip
    params[4] = (WINDOW_HEIGHT - map_height) / 2
        + z_range.max * params[2];
    // store number of rows for neighbor checks
    params[5] = rows;
}

/*
 * render_map
 * - Clear image, compute render parameters, then draw every connection in map.
 */
void	render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
    int	row;
    int	col;
    int	params[6]; // params: row, col, scale, xoff, yoff, rows

    clear_image(img); // reset to background before drawing
    if (rows == 0 || !cols)
        return ;
    init_render_params(params, points, rows, cols);
    row = 0;
    while (row < rows)
    {
        col = 0;
        while (col < cols[row])
        {
            params[0] = row; // current row index
            params[1] = col; // current column index
            draw_connections(img, points, params, cols); // draw right/down lines
            col++;
        }
        row++;
    }
}

/*
 * main (kept here for context in annotated file)
 * - Initializes MLX, creates image, and starts rendering loop.
 */
int	main(int argc, char **argv)
{
    mlx_t		*mlx;
    mlx_image_t	*img;
    t_map_data	data;

    if (argc != 2)
        return (ft_printf("Usage: ./fdf <map.fdf>\n"), 1);
    data.points = parse_map(argv[1], &data.rows, &data.cols);
    if (!data.points)
        return (1);
    mlx = init_mlx();
    if (!mlx)
        return (free_points(data.points, data.cols, data.rows), 1);
    img = create_image(mlx);
    if (!img)
        return (free_points(data.points, data.cols, data.rows), 1);
    setup_and_run(mlx, img, &data); // attach image, draw, and enter loop
    free_points(data.points, data.cols, data.rows);
    return (0);
}
