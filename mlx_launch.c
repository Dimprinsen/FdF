#include "fdf.h"
#define COS_30 0.86602540378  // cos(30°)
#define SIN_30 0.5           // sin(30°)
#define SCALE_FACTOR 1.0 // Placeholder, will be dynamically calculated
#define OFFSET_X (WINDOW_WIDTH / 2)
#define OFFSET_Y (WINDOW_HEIGHT / 2)

typedef struct s_2d_point
{
    int x;
    int y;
} t_2d_point;

// Function to calculate the isometric bounds
int calculate_isometric_bounds(t_point ***points, int rows, int *cols,
                                double *min_x, double *max_x, double *min_y, double *max_y,
                                double scale_factor)
{
    if (!points || !min_x || !max_x || !min_y || !max_y)
        return -1; // Error: Null pointer

    *min_x = *min_y = INFINITY;
    *max_x = *max_y = -INFINITY;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols[row]; col++) {
            t_point *point = points[row][col];
            double iso_x = (point->x - point->y) * COS_30 * scale_factor;
            double iso_y = (point->x + point->y) * SIN_30 * scale_factor - (point->z * scale_factor);

            if (iso_x < *min_x) *min_x = iso_x;
            if (iso_x > *max_x) *max_x = iso_x;
            if (iso_y < *min_y) *min_y = iso_y;
            if (iso_y > *max_y) *max_y = iso_y;
        }
    }

    return 0; // Success
}

// Calculate the bounding box of the map
int calculate_bounding_box(t_point ***points, int rows, int *cols, int *min_x, int *max_x, int *min_y, int *max_y, int *min_z, int *max_z)
{
    if (rows <= 0 || !points || !cols) {
        fprintf(stderr, "Invalid input to calculate_bounding_box\n");
        return -1;
    }

    *min_x = *max_x = points[0][0]->x;
    *min_y = *max_y = points[0][0]->y;
    *min_z = *max_z = points[0][0]->z;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols[row]; col++) {
            if (!points[row][col]) {
                fprintf(stderr, "Null point detected at row %d, col %d\n", row, col);
                return -1;
            }
            int x = points[row][col]->x;
            int y = points[row][col]->y;
            int z = points[row][col]->z;

            if (x < *min_x) *min_x = x;
            if (x > *max_x) *max_x = x;
            if (y < *min_y) *min_y = y;
            if (y > *max_y) *max_y = y;
            if (z < *min_z) *min_z = z;
            if (z > *max_z) *max_z = z;
        }
    }

    return 0;
}

// A function to dynamically calculate the scale factor
// Function to calculate a proper scale factor
double calculate_scale_factor(int rows, int *cols, t_point ***points, int window_width, int window_height)
{
    double min_x, max_x, min_y, max_y;

    // Start with scale factor 1.0 to calculate initial bounds
    if (calculate_isometric_bounds(points, rows, cols, &min_x, &max_x, &min_y, &max_y, 1.0) != 0) {
        fprintf(stderr, "Failed to calculate isometric bounds\n");
        return 1.0; // Default fallback scale factor
    }

    // Calculate the width and height in isometric space
    double iso_width = max_x - min_x;
    double iso_height = max_y - min_y;

    if (iso_width <= 0 || iso_height <= 0) {
        fprintf(stderr, "Invalid isometric dimensions: width=%.2f, height=%.2f\n", iso_width, iso_height);
        return 1.0; // Default fallback scale factor
    }

    // Calculate scale factor to fit within the window
    double scale_x = (double)window_width / iso_width;
    double scale_y = (double)window_height / iso_height;

    return (scale_x < scale_y) ? scale_x : scale_y; // Choose the smaller scale factor
}
/*t_2d_point isometric_projection(t_point *point)
{
    t_2d_point projected;


	//printf("Converting point: x=%d, y=%d, z=%d\n", point->x, point->y, point->z); //DEBUGGER
    projected.x = (int)((point->x - point->y) * COS_30);
    projected.y = (int)((point->x + point->y) * SIN_30 - point->z);
	//printf("Projected point: x'=%d, y'=%d\n", projected.x, projected.y); //DEBUGGER
    return (projected);
}*/

// A function to apply the projection and scaling
// Isometric projection with scaling and centering
t_2d_point isometric_projection(t_point *point, double scale_factor, int offset_x, int offset_y)
{
    t_2d_point projected;

    if (!point) {
        fprintf(stderr, "Null point passed to isometric_projection\n");
        projected.x = 0;
        projected.y = 0;
        return projected;
    }

    projected.x = (int)(((point->x - point->y) * COS_30) * scale_factor) + offset_x;
    projected.y = (int)(((point->x + point->y) * SIN_30 - point->z) * scale_factor) + offset_y;

    return projected;
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

/*void render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
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
}*/

// Render the map dynamically scaled and centered
void render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
    if (!img || !points || rows <= 0 || !cols) {
        fprintf(stderr, "Invalid input to render_map\n");
        return;
    }

    // Calculate the scale factor
    double scale_factor = calculate_scale_factor(rows, cols, points, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Calculate isometric bounds with the final scale factor
    double min_x, max_x, min_y, max_y;
    if (calculate_isometric_bounds(points, rows, cols, &min_x, &max_x, &min_y, &max_y, scale_factor) != 0) {
        fprintf(stderr, "Failed to calculate isometric bounds\n");
        return;
    }

    // Calculate dynamic offsets for centering
    int offset_x = OFFSET_X - (int)((max_x + min_x) / 2);
    int offset_y = OFFSET_Y - (int)((max_y + min_y) / 2);

    t_2d_point p1, p2;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols[row]; col++) {
            p1 = isometric_projection(points[row][col], scale_factor, offset_x, offset_y);

            // Draw a line to the right neighbor (if it exists)
            if (col + 1 < cols[row]) {
                p2 = isometric_projection(points[row][col + 1], scale_factor, offset_x, offset_y);
                draw_line(img, p1, p2, 0xFFFFFF); // White line
            }

            // Draw a line to the bottom neighbor (if it exists)
            if (row + 1 < rows && col < cols[row + 1]) {
                p2 = isometric_projection(points[row + 1][col], scale_factor, offset_x, offset_y);
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
