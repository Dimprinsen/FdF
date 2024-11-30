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

typedef struct s_render_params {
    mlx_image_t *img;
    t_point ***points;
    int rows;
    int *cols;
} t_render_params;

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

    return 0;
}

double calculate_scale_factor(int rows, int *cols, t_point ***points, int window_width, int window_height)
{
    double min_x, max_x, min_y, max_y;

    // Start with scale factor 1.0 to calculate initial bounds
    if (calculate_isometric_bounds(points, rows, cols, &min_x, &max_x, &min_y, &max_y, 1.0) != 0) {
        fprintf(stderr, "Failed to calculate isometric bounds\n");
        return 1.0; // Default fallback scale factor
    }

    // Calculate the width and height in isometric space
    double iso_width = max_x - min_x + 10; // Add margins
    double iso_height = max_y - min_y + 10;

    if (iso_width <= 0 || iso_height <= 0) {
        fprintf(stderr, "Invalid isometric dimensions: width=%.2f, height=%.2f\n", iso_width, iso_height);
        return 1.0; // Default fallback scale factor
    }

    // Calculate scale factor to fit within the window
    double scale_x = (double)window_width / iso_width;
    double scale_y = (double)window_height / iso_height;

    return (scale_x < scale_y) ? scale_x : scale_y; // Choose the smaller scale factor
}

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

void    draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2, uint32_t color)
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

    // Replacing the first for loop with a while loop
    int row = 0;
    while (row < rows) {
        int col = 0;
        // Replacing the second for loop with a while loop
        while (col < cols[row]) {
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

            col++; // Increment col to continue the while loop
        }
        row++; // Increment row to continue the outer while loop
    }
}

void error_callback(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
}

static void remove_stuff_exit(mlx_t *mlx, t_point ***points, int *cols, int rows)
{
    mlx_terminate(mlx);
    free_points(points, cols, rows);
    exit(0);
}

void key_hook(mlx_key_data_t keydata, void *param)
{
    mlx_t *mlx = (mlx_t *)param;
    static int is_terminating = 0;

    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        if (!is_terminating)
        {
            is_terminating = 1;
            remove_stuff_exit(mlx, NULL, NULL, 0);
        }
    }
}
mlx_t *init_mlx()
{
	mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
	{
		error_callback("MLX42 initialization failed");
		return (NULL);
	}
	return mlx;
}
mlx_image_t *create_image(mlx_t *mlx)
{
	mlx_image_t *img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img)
	{
		mlx_terminate(mlx);
		return (NULL);
	}
	return img;
}

t_point ***parse_map_and_check(int argc, char **argv, int *rows, int **cols)
{
    t_point ***points;

    points = parse_map(argv[1], rows, cols);
	if (argc != 2)
	{
		ft_printf("Usage: ./fdf <map.fdf>\n");
		return (NULL);
	}
	if (!points)
	{
		ft_printf("Failed to parse map\n");
		return (NULL);
	}
	return (points);
}

int main(int argc, char **argv)
{
	mlx_t *mlx;
	mlx_image_t *img;
	t_point ***points;
	int rows;
	int *cols;

	points = parse_map_and_check(argc, argv, &rows, &cols);
	if (!points)
		return (1);
	mlx = init_mlx();
	if (!mlx)
		return (1);
	img = create_image(mlx);
	if (!img)
		return (1);
	mlx_image_to_window(mlx, img, 0, 0);
	render_map(img, points, rows, cols);
	mlx_key_hook(mlx, key_hook, mlx);
	mlx_loop(mlx);

	free_points(points, cols, rows);
	return (0);
}

