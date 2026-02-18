# FDF - Comprehensive Code Evaluation Guide

## Table of Contents
1. [Data Structures](#data-structures)
2. [Main Function Flow](#main-function-flow)
3. [MLX Initialization](#mlx-initialization)
4. [The Rendering System](#the-rendering-system)
5. [3D to 2D Projection](#3d-to-2d-projection)
6. [Line Drawing Algorithm](#line-drawing-algorithm)
7. [Color System](#color-system)
8. [Event Hooks](#event-hooks)
9. [Key Concepts Summary](#key-concepts-summary)

---

## 📐 Data Structures

### Core Point Structures

#### `t_point` - 3D Point Representation
```c
typedef struct s_point
{
    int          x;        // Grid x-coordinate
    int          y;        // Grid y-coordinate  
    int          z;        // Height/elevation
    uint32_t     color;    // RGBA color (32-bit)
}   t_point;
```
This represents a 3D point in the grid. Each point has XY grid coordinates and a Z height value, plus its display color.

#### `t_2d_point` - Screen Position
```c
typedef struct s_2d_point
{
    int    x;
    int    y;
}   t_2d_point;
```
This represents a 2D screen position after 3D→2D projection.

#### `t_z_range` - Height Range
```c
typedef struct s_z_range
{
    int    min;
    int    max;
}   t_z_range;
```
Stores the minimum and maximum Z values in the map for scaling calculations.

#### `t_map_data` - Complete Map Structure
```c
typedef struct s_map_data
{
    t_point    ***points;  // 3D array: points[row][col] → pointer to t_point
    int        rows;       // Number of rows
    int        *cols;      // Array: cols[i] = number of columns in row i
}   t_map_data;
```
This holds the entire map. It's a 3-level pointer because:
- **Level 1:** Array of rows
- **Level 2:** Each row is an array of point pointers
- **Level 3:** Each pointer points to an actual `t_point`

The `cols` array allows irregular maps (different column counts per row).

---

## 🎯 Main Function Flow

### Overview - `render.c`

```c
int main(int argc, char **argv)
{
    mlx_t        *mlx;
    mlx_image_t  *img;
    t_map_data   data;
```

### Step 1: Validate Arguments
```c
    if (argc != 2)
        return (ft_printf("Usage: ./fdf <map.fdf>\n"), 1);
```
Ensures exactly one map file is provided.

### Step 2: Parse the Map
```c
    data.points = parse_map(argv[1], &data.rows, &data.cols);
    if (!data.points)
        return (1);
```
Loads and validates the map file into memory. This reads the `.fdf` file and creates the 3D point array.

### Step 3: Initialize MLX42
```c
    mlx = init_mlx();
    if (!mlx)
        return (free_points(data.points, data.cols, data.rows), 1);
```
Creates the graphics window using MLX42 library.

### Step 4: Create Image Buffer
```c
    img = create_image(mlx);
    if (!img)
        return (free_points(data.points, data.cols, data.rows), 1);
```
Creates a pixel buffer (1024×640) where the wireframe will be drawn.

### Step 5: Setup and Run
```c
    setup_and_run(mlx, img, &data);
```
Renders the map, registers event hooks, and starts the event loop.

### Step 6: Cleanup
```c
    free_points(data.points, data.cols, data.rows);
    return (0);
}
```
Frees all dynamically allocated memory.

---

## 🖼️ MLX Initialization

### `init_mlx()` - Window Creation
**File:** `mlx_launch.c`

```c
mlx_t *init_mlx(void)
{
    mlx_t *mlx;

    mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true);
    if (!mlx)
        ft_printf("Error: MLX42 initialization failed\n");
    return (mlx);
}
```

**Parameters:**
- **WINDOW_WIDTH:** 1024 pixels
- **WINDOW_HEIGHT:** 640 pixels  
- **"FdF":** Window title
- **true:** Window resizable

This creates the actual OS window using MLX42 (a modern MiniLibX implementation using OpenGL).

### `create_image()` - Image Buffer Creation
**File:** `mlx_launch.c`

```c
mlx_image_t *create_image(mlx_t *mlx)
{
    mlx_image_t *img;

    img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!img)
    {
        ft_printf("Error: Failed to create image\n");
        mlx_terminate(mlx);
    }
    return (img);
}
```

Creates a 1024×640 RGBA pixel buffer. Each pixel is 4 bytes (R, G, B, A).

**Memory layout:**
```
pixels[0..3]   = First pixel (top-left)
pixels[4..7]   = Second pixel
pixels[index]  = Pixel at position (x,y) where index = (y * width + x) * 4
```

### `setup_and_run()` - Main Loop Setup
**File:** `mlx_launch.c`

```c
void setup_and_run(mlx_t *mlx, mlx_image_t *img, t_map_data *data)
{
    mlx_image_to_window(mlx, img, 0, 0);
    render_map(img, data->points, data->rows, data->cols);
    mlx_key_hook(mlx, &key_hook, mlx);
    mlx_close_hook(mlx, &close_hook, mlx);
    mlx_loop(mlx);
    mlx_terminate(mlx);
}
```

**Step by step:**

1. **`mlx_image_to_window(mlx, img, 0, 0);`**
   - Displays the image buffer in the window at position (0,0)

2. **`render_map(...);`**
   - **This is where the magic happens!** Draws all the wireframe lines

3. **`mlx_key_hook(mlx, &key_hook, mlx);`**
   - Registers keyboard event handler (ESC to quit)

4. **`mlx_close_hook(mlx, &close_hook, mlx);`**
   - Registers window close handler (X button)

5. **`mlx_loop(mlx);`**
   - **Enters the event loop** - blocks here until window closes
   - Continuously processes events and refreshes display

6. **`mlx_terminate(mlx);`**
   - Cleanup: destroys window, frees GPU resources

---

## 🎨 The Rendering System

### `render_map()` - Main Rendering Function
**File:** `render.c`

```c
void render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)
{
    int params[6];
```

**The `params` array holds:**
- `params[0]` = current row
- `params[1]` = current column
- `params[2]` = scale factor
- `params[3]` = x offset (centering)
- `params[4]` = y offset (centering)
- `params[5]` = total rows

#### Step 1: Clear the Screen
```c
    clear_image(img);
```
Sets all pixels to black (0x000000FF).

#### Step 2: Initialize Rendering Parameters
```c
    init_render_params(params, points, rows, cols);
```
**Calculates optimal scale and centering** - ensures the entire map fits perfectly in the window.

#### Step 3: Draw All Connections
```c
    row = 0;
    while (row < rows)
    {
        col = 0;
        while (col < cols[row])
        {
            params[0] = row;
            params[1] = col;
            draw_connections(img, points, params, cols);
            col++;
        }
        row++;
    }
```

**Nested loop through every point**, drawing lines to:
- The point to the right (if exists)
- The point below (if exists)

This creates the wireframe mesh by connecting adjacent grid points.

---

### `init_render_params()` - Auto-scaling & Centering
**File:** `render.c`

```c
static void init_render_params(int *params, t_point ***pts, int rows, int *cols)
{
    int         max_cols;
    t_z_range   z_range;
    double      map_width;
    double      map_height;

    max_cols = find_max_cols(cols, rows);
    z_range = get_z_range(pts, rows, cols);
    params[2] = get_scale(rows, max_cols, z_range.max - z_range.min);
    
    map_width = (max_cols - 1 + rows - 1) * COS_30 * params[2];
    map_height = (max_cols - 1 + rows - 1) * SIN_30 * params[2]
        + (z_range.max - z_range.min) * params[2];
    
    params[3] = (WINDOW_WIDTH - map_width) / 2
        + (rows - 1) * COS_30 * params[2];
    params[4] = (WINDOW_HEIGHT - map_height) / 2
        + z_range.max * params[2];
    params[5] = rows;
}
```

#### Find Maximum Columns
```c
max_cols = find_max_cols(cols, rows);
```
Scans the `cols` array to find the widest row (needed for irregular maps).

#### Get Z Range
```c
z_range = get_z_range(pts, rows, cols);
```
**`get_z_range()` implementation:**
```c
static t_z_range get_z_range(t_point ***pts, int rows, int *cols)
{
    t_z_range range;
    int i, j;

    range.min = pts[0][0]->z;
    range.max = pts[0][0]->z;
    i = 0;
    while (i < rows)
    {
        j = 0;
        while (j < cols[i])
        {
            if (pts[i][j]->z < range.min)
                range.min = pts[i][j]->z;
            if (pts[i][j]->z > range.max)
                range.max = pts[i][j]->z;
            j++;
        }
        i++;
    }
    return (range);
}
```
Scans all points to find minimum and maximum Z values. This helps calculate how much vertical space the height variations need.

---

### `get_scale()` - Mathematical Scaling
**File:** `mlx_launch.c`

```c
int get_scale(int rows, int cols, int z_range)
{
    int     scale;
    double  scale_x;
    double  scale_y;
    double  projected_width;
    double  projected_height;

    projected_width = (cols + rows) * COS_30;
    projected_height = (cols + rows) * SIN_30 + z_range;
    
    scale_x = (WINDOW_WIDTH * 0.9) / projected_width;
    scale_y = (WINDOW_HEIGHT * 0.9) / projected_height;
    
    if (scale_x < scale_y)
        scale = (int)scale_x;
    else
        scale = (int)scale_y;
    
    if (scale < 1)
        scale = 1;
    
    return (scale);
}
```

#### The Math Behind Isometric Projection

Your map uses **isometric projection** (30° angle). In this view:
- Moving 1 unit in X moves the screen point by `cos(30°) ≈ 0.866` horizontally
- Moving 1 unit in Y moves the screen point by `cos(30°)` horizontally (opposite direction)
- Combined X+Y movement creates the diagonal grid

**Calculate Projected Dimensions:**
```c
projected_width = (cols + rows) * COS_30;
```
Maximum width occurs at the corner where both X and Y are at their max:
- `COS_30 = 0.86602540378` (defined in header)
- Total diagonal span = `(cols + rows) * 0.866`

```c
projected_height = (cols + rows) * SIN_30 + z_range;
```
Height includes:
- Base isometric height: `(cols + rows) * 0.5` (SIN_30 = 0.5)
- Plus vertical displacement from Z values

**Calculate Scale Factors:**
```c
scale_x = (WINDOW_WIDTH * 0.9) / projected_width;
scale_y = (WINDOW_HEIGHT * 0.9) / projected_height;
```
**90% of window size** (0.9 factor) provides 10% margins around the map.

**Choose Limiting Factor:**
```c
if (scale_x < scale_y)
    scale = (int)scale_x;
else
    scale = (int)scale_y;
```
**Use the smaller scale** to ensure the entire map fits both horizontally AND vertically.

#### Calculate Centering Offsets

```c
map_width = (max_cols - 1 + rows - 1) * COS_30 * params[2];
map_height = (max_cols - 1 + rows - 1) * SIN_30 * params[2]
    + (z_range.max - z_range.min) * params[2];
```
Calculate actual pixel dimensions of the scaled map.

```c
params[3] = (WINDOW_WIDTH - map_width) / 2
    + (rows - 1) * COS_30 * params[2];
```
**X offset:** Centers horizontally, then adjusts for the fact that grid point (0,0) appears left of center in isometric view.

```c
params[4] = (WINDOW_HEIGHT - map_height) / 2
    + z_range.max * params[2];
```
**Y offset:** Centers vertically, accounting for Z displacement from the top.

---

### `draw_connections()` - Drawing Lines Between Points
**File:** `render.c`

```c
void draw_connections(mlx_image_t *img, t_point ***pts, int *p, int *cols)
{
    t_2d_point p1, p2;

    // Project current point to 2D screen coordinates
    p1 = project_and_scale(pts[p[0]][p[1]], p[2]);
    p1.x = p1.x + p[3];
    p1.y = p1.y + p[4];

    // Draw line to right neighbor (if exists)
    if (p[1] + 1 < cols[p[0]])
    {
        p2 = project_and_scale(pts[p[0]][p[1] + 1], p[2]);
        p2.x = p2.x + p[3];
        p2.y = p2.y + p[4];
        draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
    }

    // Draw line to bottom neighbor (if exists)
    if (p[0] + 1 < p[5] && p[1] < cols[p[0] + 1])
    {
        p2 = project_and_scale(pts[p[0] + 1][p[1]], p[2]);
        p2.x = p2.x + p[3];
        p2.y = p2.y + p[4];
        draw_line(img, p1, p2, pts[p[0]][p[1]]->color);
    }
}
```

**What this does:**
1. **Projects current point** from 3D grid space to 2D screen space
2. **Adds centering offsets** to position in window
3. **Draws line to right neighbor** (same row, next column)
4. **Draws line to bottom neighbor** (next row, same column)

**Why only right and down?**
- Each point connects to 4 neighbors in a grid
- Drawing only right+down avoids duplicate lines
- Previous points already drew lines to this point from left+up

**Boundary checking:**
- `p[1] + 1 < cols[p[0]]`: Right neighbor exists in current row
- `p[0] + 1 < p[5]`: Not on last row
- `p[1] < cols[p[0] + 1]`: Next row has enough columns (irregular map support)

---

## 🔷 3D to 2D Projection

### `project_and_scale()` - Isometric Projection
**File:** `draw_utils.c`

```c
t_2d_point project_and_scale(t_point *point, int scale)
{
    t_2d_point projected;

    projected.x = (int)((point->x - point->y) * COS_30 * scale);
    projected.y = (int)((point->x + point->y) * SIN_30 * scale
            - point->z * scale);
    return (projected);
}
```

**This is the heart of isometric projection!**

### X Calculation
```c
projected.x = (point->x - point->y) * COS_30 * scale
```
- In isometric view, X moves diagonally right-down
- Y moves diagonally left-down  
- `x - y` creates the horizontal axis
- Multiplied by `cos(30°) = 0.866` for the 30° rotation
- Multiplied by scale for zoom level

### Y Calculation
```c
projected.y = (point->x + point->y) * SIN_30 * scale - point->z * scale
```
- `x + y` creates the depth diagonal
- Multiplied by `sin(30°) = 0.5` for vertical foreshortening
- `- point->z * scale` moves point UP based on height (higher Z = smaller Y coordinate)

### Visual Representation

```
       Grid (0,0,0)          Grid (1,0,0)
            ●──────────────────────●
             \                    /
              \                  /
               \                /
                \              /
                 \            /
                  \          /
                   \        /
                    \      /
                     \    /
                      \  /
                       ●
                  Grid (0,1,0)

If point has z > 0, it's drawn higher (smaller y)
If point has z < 0, it's drawn lower (larger y)
```

### Projection Mathematics

**Standard isometric projection formula:**
```
screen_x = (grid_x - grid_y) * cos(30°)
screen_y = (grid_x + grid_y) * sin(30°) - grid_z
```

This creates a 30° angle view where:
- X-axis points to 330° (bottom-right)
- Y-axis points to 210° (bottom-left)
- Z-axis points straight up

---

## 📏 Line Drawing Algorithm

### `draw_line()` - Bresenham's Algorithm
**File:** `draw_utils.c`

```c
void draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2, uint32_t color)
{
    int dx, dy, err, e2;

    dx = abs(p2.x - p1.x);
    dy = abs(p2.y - p1.y);
    err = dx - dy;

    while (1)
    {
        put_pixel(img, p1.x, p1.y, color);
        
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        
        e2 = err * 2;
        
        if (e2 > -dy)
            step_x(&p1, p2, &err, dy);
        
        if (e2 < dx)
            step_y(&p1, p2, &err, dx);
    }
}
```

### Algorithm Explanation

**Initialize:**
```c
dx = abs(p2.x - p1.x);  // Absolute horizontal distance
dy = abs(p2.y - p1.y);  // Absolute vertical distance
err = dx - dy;          // Error accumulator
```

**The error accumulator** is the key to Bresenham's algorithm:
- When drawing a line, you can't draw fractional pixels
- Bresenham uses integer math to decide when to step in X vs Y
- The error tracks how far off the ideal line we are

**Main Loop:**
1. **Draw current pixel**
2. **Check if we've reached the end**
3. **Calculate error correction needs** (`e2 = err * 2`)
4. **Step in X if error indicates** (`e2 > -dy`)
5. **Step in Y if error indicates** (`e2 < dx`)

**Important:** Both conditions can be true simultaneously → diagonal step!

### Helper Functions

#### `step_x()` - Horizontal Step
**File:** `draw_utils.c`

```c
void step_x(t_2d_point *p1, t_2d_point p2, int *err, int dy)
{
    *err -= dy;
    if (p1->x < p2.x)
        p1->x++;
    else
        p1->x--;
}
```
- Moves X coordinate toward target
- Adjusts error accumulator by `-dy`
- Handles both left-to-right and right-to-left

#### `step_y()` - Vertical Step
**File:** `draw_utils.c`

```c
void step_y(t_2d_point *p1, t_2d_point p2, int *err, int dx)
{
    *err += dx;
    if (p1->y < p2.y)
        p1->y++;
    else
        p1->y--;
}
```
- Moves Y coordinate toward target
- Adjusts error accumulator by `+dx`
- Handles both top-to-bottom and bottom-to-top

### `put_pixel()` - Direct Pixel Manipulation
**File:** `draw_utils.c`

```c
void put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
    uint8_t *pixel;
    int     index;

    if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
    {
        index = (y * img->width + x) * sizeof(int32_t);
        pixel = &img->pixels[index];
        *(pixel++) = (uint8_t)(color >> 24);  // Red
        *(pixel++) = (uint8_t)(color >> 16);  // Green
        *(pixel++) = (uint8_t)(color >> 8);   // Blue
        *(pixel++) = (uint8_t)(color & 0xFF); // Alpha
    }
}
```

#### Bounds Checking
```c
if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
```
**Critical!** Prevents writing outside image buffer:
- Buffer overflow = crash or memory corruption
- Always validate coordinates before writing

#### Index Calculation
```c
index = (y * img->width + x) * sizeof(int32_t);
```
**Convert 2D coordinates to 1D array index:**
- `y * img->width`: Skip to correct row
- `+ x`: Offset within row
- `* 4` (sizeof int32_t): Each pixel is 4 bytes (RGBA)

**Example:** Pixel at (5, 3) in 1024-width image:
```
index = (3 * 1024 + 5) * 4 = 12,308
```

#### Color Byte Extraction
```c
*(pixel++) = (uint8_t)(color >> 24);  // R: bits 24-31
*(pixel++) = (uint8_t)(color >> 16);  // G: bits 16-23
*(pixel++) = (uint8_t)(color >> 8);   // B: bits 8-15
*(pixel++) = (uint8_t)(color & 0xFF); // A: bits 0-7
```

**Color format:** `0xRRGGBBAA` (32-bit integer)

**Example:** `0xFF5500AA` (orange, semi-transparent)
- Red byte: `0xFF5500AA >> 24 = 0xFF` (255)
- Green byte: `0xFF5500AA >> 16 & 0xFF = 0x55` (85)
- Blue byte: `0xFF5500AA >> 8 & 0xFF = 0x00` (0)
- Alpha byte: `0xFF5500AA & 0xFF = 0xAA` (170)

---

## 🎨 Color System

### `parse_color()` - Color String Parser
**File:** `color_utils.c`

```c
uint32_t parse_color(char *str)
{
    char *comma;

    comma = ft_strchr(str, ',');
    if (comma && *(comma + 1) == '0' && *(comma + 2) == 'x')
        return (parse_hex_color(comma + 3));
    return (0x40E0D0FF);  // Default: Turquoise
}
```

**Input format examples:**
- `"10"` → Default color (turquoise)
- `"10,0xFF0000"` → Red color
- `"5,0x00FF00"` → Green color

**Algorithm:**
1. Search for comma in string
2. Check if followed by `"0x"` (hex indicator)
3. If yes: Parse hex color
4. If no: Return default turquoise (`0x40E0D0FF`)

### `parse_hex_color()` - Hex String to Integer
**File:** `color_utils.c`

```c
static uint32_t parse_hex_color(char *str)
{
    uint32_t result;
    int i;

    result = 0;
    i = 0;
    while (str[i] && i < 8)
    {
        result = result * 16 + ft_hexdigit(str[i]);
        i++;
    }
    return ((result << 8) | 0xFF);
}
```

**Conversion process:**
```
Input: "FF5500"
Step 1: F → result = 15
Step 2: F → result = 15 * 16 + 15 = 255
Step 3: 5 → result = 255 * 16 + 5 = 4085
Step 4: 5 → result = 4085 * 16 + 0 = 65360
Step 5: 0 → result = 65360 * 16 + 0 = 1045760
Step 6: 0 → result = 1045760 * 16 + 0 = 16732160
Result in hex: 0xFF5500
```

**Add alpha channel:**
```c
return ((result << 8) | 0xFF);
```
- Input: `0xRRGGBB` (24-bit RGB)
- Left shift 8 bits: `0xRRGGBB00`
- OR with `0xFF`: `0xRRGGBBFF` (full opacity)

### `ft_hexdigit()` - Character to Hex Value
**File:** `color_utils.c`

```c
static int ft_hexdigit(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');        // '0'-'9' → 0-9
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);   // 'a'-'f' → 10-15
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);   // 'A'-'F' → 10-15
    return (0);
}
```

**Conversion table:**
```
'0' → 0    'A' or 'a' → 10
'1' → 1    'B' or 'b' → 11
'2' → 2    'C' or 'c' → 12
'3' → 3    'D' or 'd' → 13
'4' → 4    'E' or 'e' → 14
'5' → 5    'F' or 'f' → 15
'6' → 6
'7' → 7
'8' → 8
'9' → 9
```

---

## ⌨️ Event Hooks

### `key_hook()` - Keyboard Event Handler
**File:** `hooks.c`

```c
void key_hook(mlx_key_data_t keydata, void *param)
{
    mlx_t *mlx;

    mlx = (mlx_t *)param;
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(mlx);
}
```

**Called every time a key event occurs:**
- `keydata.key`: Which key (MLX_KEY_ESCAPE, MLX_KEY_A, etc.)
- `keydata.action`: Type of event
  - `MLX_PRESS`: Key was pressed down
  - `MLX_RELEASE`: Key was released
  - `MLX_REPEAT`: Key is being held (auto-repeat)
- `param`: User data passed during hook registration (mlx pointer)

**Current functionality:**
- Detects ESC key press
- Closes the window gracefully

**Extension possibilities:**
- Arrow keys for rotation
- +/- keys for zoom
- WASD for panning
- R key for reset view

### `close_hook()` - Window Close Handler
**File:** `hooks.c`

```c
void close_hook(void *param)
{
    mlx_t *mlx;

    mlx = (mlx_t *)param;
    mlx_close_window(mlx);
}
```

**Called when:**
- User clicks the X button
- Window receives close request from OS
- Application code requests window close

**Purpose:**
- Allows graceful cleanup before termination
- Could save state, prompt user, etc.
- Currently just closes immediately

---

## 🧠 Key Concepts Summary

### 1. Memory Architecture
**Triple pointer for 2D grid of points:**
```
t_point ***points
    ↓
    [row 0] → [col 0] → t_point {x:0, y:0, z:5, color:...}
              [col 1] → t_point {x:1, y:0, z:3, color:...}
              [col 2] → t_point {x:2, y:0, z:7, color:...}
    [row 1] → [col 0] → t_point {x:0, y:1, z:2, color:...}
              [col 1] → t_point {x:1, y:1, z:4, color:...}
    [row 2] → [col 0] → t_point {x:0, y:2, z:6, color:...}
              ...
```

### 2. Isometric Projection
**30° rotation using trigonometry:**
```
Screen X = (Grid X - Grid Y) × cos(30°)
Screen Y = (Grid X + Grid Y) × sin(30°) - Grid Z
```

**Constants:**
- `COS_30 = 0.86602540378`
- `SIN_30 = 0.5`

**Effect:** Creates a diamond-shaped grid viewed from 30° angle

### 3. Auto-scaling Algorithm
**Ensures any size map fits in window:**
1. Calculate projected dimensions of the map
2. Determine scale factors for width and height
3. Choose the smaller scale (limiting factor)
4. Calculate centering offsets
5. Apply scale and offsets to all points

### 4. Bresenham's Algorithm
**Efficient integer-only line drawing:**
- No floating-point math
- No division operations
- Only addition, subtraction, multiplication by 2 (bit shift)
- Guarantees connected pixels (no gaps)

### 5. Direct Pixel Manipulation
**Writing RGBA bytes to image buffer:**
- Each pixel = 4 consecutive bytes
- Order: Red, Green, Blue, Alpha
- Index = `(y × width + x) × 4`
- Bounds checking prevents crashes

### 6. Event-driven Programming
**Hooks respond to user input:**
- Keyboard hook: Key press/release events
- Close hook: Window close events
- Loop: Continuously processes events
- Asynchronous: Doesn't block main logic

### 7. Graph Traversal
**Each point draws lines to adjacent neighbors:**
```
For each point (x, y):
    If point (x+1, y) exists: Draw line right
    If point (x, y+1) exists: Draw line down
```
**Result:** Complete wireframe mesh without duplicate lines

### 8. Color Format
**32-bit RGBA:**
```
0xRRGGBBAA
  ││││││└└─ Alpha (transparency)
  ││││└└─── Blue
  ││└└───── Green
  └└─────── Red
```
- `0xFF000000` = Opaque red
- `0x00FF0000` = Opaque green
- `0x0000FF00` = Opaque blue
- `0xFFFFFF00` = Opaque white
- `0x00000000` = Transparent black
- `0xFFFFFFFF` = Opaque white

---

## 🔍 Data Flow Summary

### Complete Rendering Pipeline

```
1. Map File (.fdf)
        ↓
2. Parse → t_point*** (3D grid in memory)
        ↓
3. Find Z range (min/max heights)
        ↓
4. Calculate optimal scale
        ↓
5. Calculate centering offsets
        ↓
6. For each point:
        ↓
   a. Project 3D → 2D (isometric)
        ↓
   b. Apply scale
        ↓
   c. Apply centering offset
        ↓
   d. Draw lines to neighbors
        ↓
      i. Bresenham's algorithm
        ↓
     ii. Put pixels in buffer
        ↓
7. Display image in window
        ↓
8. Event loop (keyboard, close)
```

---

## 📊 Performance Characteristics

### Time Complexity
- **Parsing:** O(n) where n = number of points
- **Z range finding:** O(n)
- **Rendering:** O(n) - each point processed once
- **Line drawing:** O(L) where L = length of line in pixels
- **Overall:** O(n + L×n) ≈ O(n) for typical maps

### Space Complexity
- **Point storage:** O(n) - one t_point per grid position
- **Image buffer:** O(W×H) where W=1024, H=640 (constant)
- **Rows array:** O(r) where r = number of rows
- **Cols array:** O(r)
- **Overall:** O(n) where n dominates

### Optimization Techniques
1. **Integer-only arithmetic** in Bresenham's (no float/double)
2. **Single pass rendering** (no multi-layer compositing)
3. **Bounds checking** prevents expensive error handling
4. **Direct memory access** via pixel buffer
5. **No redundant calculations** (scale cached, offsets precomputed)

---

## 🎓 Evaluation Checklist

### Core Functionality
- ✅ Reads and parses .fdf map files
- ✅ Handles irregular maps (different row lengths)
- ✅ Supports custom colors per point
- ✅ Automatic scaling to fit any map size
- ✅ Centers map in window
- ✅ Isometric projection (30° angle)
- ✅ Wireframe rendering
- ✅ ESC key to exit
- ✅ Window close button works

### Code Quality
- ✅ Proper memory management (malloc/free)
- ✅ Error checking (file I/O, MLX)
- ✅ Bounds checking (pixel writes)
- ✅ Norminette compliant structure
- ✅ Modular design (separate files)
- ✅ Clear function purposes
- ✅ Efficient algorithms

### Graphics Techniques
- ✅ Isometric projection math
- ✅ Bresenham's line algorithm
- ✅ Direct pixel buffer manipulation
- ✅ RGBA color format
- ✅ Automatic viewport calculation
- ✅ Coordinate transformation pipeline

### Understanding Points
- **Data structures:** Triple pointer array for grid
- **Projection:** 3D to 2D conversion using trig
- **Scaling:** Fit-to-window algorithm
- **Line drawing:** Classic computer graphics algorithm
- **Event handling:** Callback-based architecture
- **Memory layout:** Sequential RGBA pixel buffer

---

## 🚀 Potential Extensions

### Features to Add
1. **Rotation controls** (arrow keys)
2. **Zoom in/out** (+/- keys)
3. **Pan view** (WASD keys)
4. **Toggle perspective/isometric** (P key)
5. **Height scaling** (adjust Z multiplier)
6. **Color gradients** (height-based coloring)
7. **Multiple projections** (top-down, side view)
8. **Wireframe/filled toggle**
9. **Save screenshot**
10. **Animation** (rotate automatically)

### Advanced Topics
- **Perspective projection** (vanishing points)
- **Hidden line removal** (Z-buffering)
- **Anti-aliasing** (smooth lines)
- **Lighting/shading** (3D appearance)
- **Texture mapping**
- **Real-time rotation** (matrix transformations)

---

## 📖 References

### MLX42 Documentation
- Window management
- Image manipulation
- Event hooks
- Pixel buffer format

### Algorithms
- **Bresenham's Line Algorithm** (1965)
- **Isometric Projection** (technical drawing)
- **Graph traversal** (visiting nodes)

### Mathematical Concepts
- Trigonometry (sin, cos)
- Coordinate transformation
- Scale factors
- Linear interpolation

---

This evaluation guide provides a comprehensive understanding of how the FDF wireframe renderer works, from low-level pixel manipulation to high-level architectural decisions. The program successfully demonstrates fundamental computer graphics concepts: coordinate transformations, projections, line rasterization, and event-driven programming.
