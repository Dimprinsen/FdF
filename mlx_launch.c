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

int calculate_isometric_bounds(t_env *env)
{
    if (!env->points || !env->min_x || !env->max_x || !env->min_y || !env->max_y)
        return -1; // Error: Null pointer

    env->min_x = env->min_y = INFINITY;
    env->max_x = env->max_y = -INFINITY;

    for (int row = 0; row < env->rows; row++) {
        for (int col = 0; col < env->cols[row]; col++) {
            t_point *point = env->points[row][col];
            double iso_x = (point->x - point->y) * COS_30 * env->scale_factor;
            double iso_y = (point->x + point->y) * SIN_30 * env->scale_factor - (point->z * env->scale_factor);

            if (iso_x < env->min_x) env->min_x = iso_x;
            if (iso_x > env->max_x) env->max_x = iso_x;
            if (iso_y < env->min_y) env->min_y = iso_y;
            if (iso_y > env->max_y) env->max_y = iso_y;
        }
    }

    return 0;
}


double calculate_scale_factor(t_env *env, int window_width, int window_height)
{
    double iso_width, iso_height;

    // Start with scale factor 1.0 to calculate initial bounds
    if (calculate_isometric_bounds(env) != 0) {
        fprintf(stderr, "Failed to calculate isometric bounds\n");
        return 1.0; // Default fallback scale factor
    }

    // Calculate the width and height in isometric space
    iso_width = env->max_x - env->min_x + 10; // Add margins
    iso_height = env->max_y - env->min_y + 10;

    if (iso_width <= 0 || iso_height <= 0) {
        fprintf(stderr, "Invalid isometric dimensions: width=%.2f, height=%.2f\n", iso_width, iso_height);
        return 1.0; // Default fallback scale factor
    }

    // Calculate scale factor to fit within the window
    double scale_x = (double)window_width / iso_width;
    double scale_y = (double)window_height / iso_height;

    return (scale_x < scale_y) ? scale_x : scale_y; // Choose the smaller scale factor
}


t_2d_point isometric_projection(t_point *point, t_env *env)
{
    t_2d_point projected;

    if (!point) {
        fprintf(stderr, "Null point passed to isometric_projection\n");
        projected.x = 0;
        projected.y = 0;
        return projected;
    }

    projected.x = (int)(((point->x - point->y) * COS_30) * env->scale_factor) + OFFSET_X;
    projected.y = (int)(((point->x + point->y) * SIN_30 - point->z) * env->scale_factor) + OFFSET_Y;

    return projected;
}



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


void render_map(t_env *env)
{
    if (!env->img || !env->points || env->rows <= 0 || !env->cols) {
        fprintf(stderr, "Invalid input to render_map\n");
        return;
    }

    // Calculate the scale factor
    env->scale_factor = calculate_scale_factor(env, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Calculate isometric bounds with the final scale factor
    if (calculate_isometric_bounds(env) != 0) {
        fprintf(stderr, "Failed to calculate isometric bounds\n");
        return;
    }
    t_2d_point p1, p2;

    int row = 0;
    while (row < env->rows) {
        int col = 0;
        while (col < env->cols[row]) {
            p1 = isometric_projection(env->points[row][col], env);

            if (col + 1 < env->cols[row]) {
                p2 = isometric_projection(env->points[row][col + 1], env);
                draw_line(env->img, p1, p2, 0xFFFFFF); // White line
            }

            if (row + 1 < env->rows && col < env->cols[row + 1]) {
                p2 = isometric_projection(env->points[row + 1][col], env);
                draw_line(env->img, p1, p2, 0xFFFFFF); // White line
            }

            col++;
        }
        row++;
    }
}



void error_callback(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
}

static void remove_stuff_exit(mlx_t *mlx, t_env *env)
{
    mlx_terminate(mlx);
    if (env)
        free_points(env->points, env->cols, env->rows);
    exit(0);
}

void key_hook(mlx_key_data_t keydata, void *param)
{
    static int is_terminating = 0;

    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        if (!is_terminating)
        {
            is_terminating = 1;
            remove_stuff_exit((mlx_t *)param, NULL); // Use param directly
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

t_point ***parse_map_and_check(t_env *env, int argc, char **argv)
{
    t_point ***points;

    points = parse_map(argv[1], env);
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
    env->points = points;  // Store points in the env struct
    return (points);
}

int main(int argc, char **argv)
{
    t_env env;
    mlx_t *mlx;
    mlx_image_t *img;

    // Initialize the environment
    if (parse_map_and_check(&env, argc, argv) == NULL)
        return (1);

    mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
    if (!mlx)
    {
        error_callback("MLX42 initialization failed");
        return (1);
    }
    env.mlx = mlx;

    img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!img)
    {
        mlx_terminate(mlx);
        return (1);
    }
    env.img = img;

    mlx_image_to_window(mlx, img, 0, 0);
    render_map(&env);

    mlx_key_hook(mlx, key_hook, mlx);
    mlx_loop(mlx);

    free_points(env.points, env.cols, env.rows);
    mlx_terminate(mlx);
    return (0);
}



