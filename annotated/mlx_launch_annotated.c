/* ************************************************************************** */
/* Annotated copy of mlx_launch.c — MLX initialization and image handling */
/* ************************************************************************** */

#include "fdf.h"

/*
 * get_scale
 * - Compute a pixel scale so the projected map fits into the window.
 */
int	get_scale(int rows, int cols, int z_range)
{
    int		scale;
    double	scale_x;
    double	scale_y;
    double	projected_width;
    double	projected_height;

    // approximate projected dimensions in grid units using trig constants
    projected_width = (cols + rows) * COS_30;
    projected_height = (cols + rows) * SIN_30 + z_range;
    // compute scale factors to fit 90% of window in each axis
    scale_x = (WINDOW_WIDTH * 0.9) / projected_width;
    scale_y = (WINDOW_HEIGHT * 0.9) / projected_height;
    if (scale_x < scale_y)
        scale = (int)scale_x;
    else
        scale = (int)scale_y;
    if (scale < 1)
        scale = 1; // ensure at least 1 pixel per unit
    return (scale);
}

/*
 * find_max_cols
 * - Return the maximum number of columns across rows (ragged maps allowed).
 */
int	find_max_cols(int *cols, int rows)
{
    int	i;
    int	max;

    i = 0;
    max = 0;
    while (i < rows)
    {
        if (cols[i] > max)
            max = cols[i];
        i++;
    }
    return (max);
}

/*
 * setup_and_run
 * - Attach image to window, render initial frame, register hooks, and
 *   start the MLX event loop.
 */
void	setup_and_run(mlx_t *mlx, mlx_image_t *img, t_map_data *data)
{
    mlx_image_to_window(mlx, img, 0, 0); // place image at window origin
    render_map(img, data->points, data->rows, data->cols); // draw once
    mlx_key_hook(mlx, &key_hook, mlx); // handle key events
    mlx_close_hook(mlx, &close_hook, mlx); // handle window close
    mlx_loop(mlx); // enter event loop
    mlx_terminate(mlx); // cleanup when loop finishes
}

/*
 * create_image
 * - Allocate a new MLX image of given width/height and return pointer.
 * - On failure prints an error and terminates MLX.
 */
mlx_image_t	*create_image(mlx_t *mlx)
{
    mlx_image_t	*img;

    img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT); // allocate
    if (!img)
    {
        ft_printf("Error: Failed to create image\n");
        mlx_terminate(mlx);
    }
    return (img);
}

/*
 * init_mlx
 * - Initialize MLX window and return the context pointer.
 */
mlx_t	*init_mlx(void)
{
    mlx_t	*mlx;

    mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true);
    if (!mlx)
        ft_printf("Error: MLX42 initialization failed\n");
    return (mlx);
}
