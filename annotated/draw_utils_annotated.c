/* ************************************************************************** */
/* Annotated copy of draw_utils.c — line-by-line comments explaining behavior */
/* ************************************************************************** */

#include "fdf.h"

/*
 * project_and_scale
 * - Convert a 3D grid point (x,y,z) into 2D isometric screen coordinates.
 */
t_2d_point	project_and_scale(t_point *point, int scale)
{
    t_2d_point	projected; // result to return

    // X: (x - y) rotated by cos(30°) then scaled to pixels
    projected.x = (int)((point->x - point->y) * COS_30 * scale);

    /*
     * Y: (x + y) collapsed by sin(30°) then scaled, minus z*scale to
     * account for elevation (z lifts the point upward on screen).
     */
    projected.y = (int)((point->x + point->y) * SIN_30 * scale
            - point->z * scale);
    return (projected);
}

/*
 * step_x
 * - Advance the current point's X towards the target and update the
 *   Bresenham error accumulator.
 */
void	step_x(t_2d_point *p1, t_2d_point p2, int *err, int dy)
{
    *err -= dy;              // decrement error by dy (decision term)
    if (p1->x < p2.x)
        p1->x++;             // move right if target is to the right
    else
        p1->x--;             // otherwise move left
}

/*
 * step_y
 * - Advance the current point's Y towards the target and update the
 *   Bresenham error accumulator.
 */
void	step_y(t_2d_point *p1, t_2d_point p2, int *err, int dx)
{
    *err += dx;              // increment error by dx (decision term)
    if (p1->y < p2.y)
        p1->y++;             // move down if target Y is larger
    else
        p1->y--;             // otherwise move up
}

/*
 * put_pixel
 * - Write a 32-bit RGBA color into the image buffer at (x,y) if inside
 *   bounds. The image stores pixels as 4 bytes per pixel in R,G,B,A order.
 */
void	put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
    uint8_t	*pixel; // pointer to the pixel's first byte in the buffer
    int	index;     // byte index into img->pixels

    // Bounds check to prevent out-of-range writes
    if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
    {
        // row-major linear index, times 4 bytes per pixel
        index = (y * img->width + x) * sizeof(int32_t);
        pixel = &img->pixels[index];

        // Write bytes in order: R, G, B, A — high byte to low byte
        *(pixel++) = (uint8_t)(color >> 24); // R
        *(pixel++) = (uint8_t)(color >> 16); // G
        *(pixel++) = (uint8_t)(color >> 8);  // B
        *(pixel++) = (uint8_t)(color & 0xFF); // A
    }
}

/*
 * draw_line
 * - Integer Bresenham-style line drawing between p1 and p2.
 * - Uses dx/dy absolute values with an error accumulator to decide
 *   whether to step in X, Y, or both.
 */
void	draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2,
    uint32_t color)
{
    int	dx; // absolute horizontal distance
    int	dy; // absolute vertical distance
    int	err; // error accumulator (decision variable)
    int	e2;  // temporary twice-error used for comparisons

    dx = abs(p2.x - p1.x);
    dy = abs(p2.y - p1.y);
    err = dx - dy; // initial decision value
    while (1)
    {
        put_pixel(img, p1.x, p1.y, color); // draw current pixel
        if (p1.x == p2.x && p1.y == p2.y)
            break ; // reached destination
        e2 = err * 2; // double the error term for comparison
        if (e2 > -dy)
            step_x(&p1, p2, &err, dy); // conditionally move in X
        if (e2 < dx)
            step_y(&p1, p2, &err, dx); // conditionally move in Y
    }
}
