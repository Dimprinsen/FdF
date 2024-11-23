#include "fdf.h"
#define COS_30 0.86602540378  // cos(30°)
#define SIN_30 0.5           // sin(30°)

typedef struct s_2d_point
{
    int x;
    int y;
} t_2d_point;

t_2d_point isometric_projection(t_point *point)
{
    t_2d_point projected;


	printf("Converting point: x=%d, y=%d, z=%d\n", point->x, point->y, point->z); //DEBUGGER
    projected.x = (int)((point->x - point->y) * COS_30);
    projected.y = (int)((point->x + point->y) * SIN_30 - point->z);
	printf("Projected point: x'=%d, y'=%d\n", projected.x, projected.y); //DEBUGGER
    return (projected);
}

//Error call from main
void error_callback(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
}

//Drawing the line.

void draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2, uint32_t color)
{
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        mlx_put_pixel(img, p1.x, p1.y, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            p1.x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            p1.y += sy;
        }
    }
}

// Function to clear the image (set all pixels to black or a background color)
void clear_image(mlx_image_t *img)
{
    // Use uint32_t for the loop variables to match the type of img->width and img->height
    for (uint32_t y = 0; y < img->height; y++)  // Use uint32_t for y
    {
        for (uint32_t x = 0; x < img->width; x++)  // Use uint32_t for x
        {
            mlx_put_pixel(img, x, y, 0x000000);  // Set all pixels to black (or any background color)
        }
    }
}
//Rendering the map

void render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
    int row, col;
    t_2d_point p1, p2;

	// Clear the image first to avoid remnants of previous renders
    clear_image(img);

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols[row]; col++)
        {
            // Project the current point
            p1 = isometric_projection(points[row][col]);
			// Scale and offset to fit within the window size (if needed)
            p1.x = p1.x + (WINDOW_WIDTH / 2);  // Adjust the X offset if necessary
            p1.y = p1.y + (WINDOW_HEIGHT / 2); // Adjust the Y offset if necessary

            // Draw a line to the right neighbor (if it exists)
            if (col + 1 < cols[row])
            {
                p2 = isometric_projection(points[row][col + 1]);
				p2.x = p2.x + (WINDOW_WIDTH / 2);  // Adjust X offset
                p2.y = p2.y + (WINDOW_HEIGHT / 2); // Adjust Y offset
                draw_line(img, p1, p2, 0xFFFFFF); // White line
            }

            // Draw a line to the bottom neighbor (if it exists)
            if (row + 1 < rows && col < cols[row + 1])
            {
                p2 = isometric_projection(points[row + 1][col]);
				p2.x = p2.x + (WINDOW_WIDTH / 2);  // Adjust X offset
                p2.y = p2.y + (WINDOW_HEIGHT / 2); // Adjust Y offset
                draw_line(img, p1, p2, 0xFFFFFF); // White line
            }
        }
    }
}

//Creating the window and setting up the main event loop.
int main(int argc, char **argv)
{
    mlx_t *mlx;
    mlx_image_t *img;
    t_point ***points;
    int rows;
    int *cols;

    if (argc != 2)
    {
        ft_printf("Usage: ./fdf <map.fdf>\n");
        return (1);
    }

    // Parse the map
    points = parse_map(argv[1], &rows, &cols);

    // Initialize MLX42
    mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
    if (!mlx)
    {
        error_callback("MLX42 initialization failed");
        return (1);
    }

    // Create an image for rendering
    img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!img)
    {
        fprintf(stderr, "Failed to create image\n");
        mlx_terminate(mlx);
        return (1);
    }
    mlx_image_to_window(mlx, img, 0, 0);

    // Render the map
    render_map(img, points, rows, cols);

    // Main loop
    mlx_loop(mlx);
    mlx_terminate(mlx);

    // Free resources
    free_points(points, cols, rows);
    return (0);
}
