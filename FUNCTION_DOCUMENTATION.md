# FdF - Wireframe 3D Map Renderer - Complete Function Documentation

## Table of Contents
1. [Program Overview](#program-overview)
2. [Program Flow](#program-flow)
3. [Data Structures](#data-structures)
4. [File-by-File Function Breakdown](#file-by-file-function-breakdown)
   - [fdf.c - Map Parsing](#fdfc---map-parsing)
   - [fdf_utils.c - Parsing Utilities](#fdf_utilsc---parsing-utilities)
   - [mlx_launch.c - Main Program](#mlx_launchc---main-program)
   - [render.c - Rendering Engine](#renderc---rendering-engine)
   - [draw_utils.c - Drawing Primitives](#draw_utilsc---drawing-primitives)
   - [color_utils.c - Color Parsing](#color_utilsc---color-parsing)
   - [hooks.c - Event Handling](#hooksc---event-handling)
5. [Algorithm Explanations](#algorithm-explanations)

---

## Program Overview

**FdF (Fil de Fer)** is a 3D wireframe renderer that takes a .fdf map file (grid of elevation values) and displays it as an isometric wireframe projection on screen.

**Key Features:**
- Reads map files with elevation and optional color data
- Uses isometric projection (30-degree angle)
- Dynamic scaling based on map dimensions
- Handles maps with varying row lengths
- Proper centering accounting for elevation
- Memory-safe with comprehensive error handling

**Technical Specifications:**
- Window size: 1024x640 pixels
- Projection: Isometric (cos 30° = 0.866, sin 30° = 0.5)
- Graphics library: MLX42
- Drawing algorithm: Bresenham's line algorithm
- Default color: White (0xFFFFFFFF)

---

## Program Flow

```
1. main() validates arguments
2. parse_map() reads and parses the .fdf file
   ├── count_rows() counts total rows
   ├── allocate_points() allocates memory
   ├── read_map_lines() processes each line
   │   └── parse_row() creates point structures
   └── Returns 3D array of points
3. init_mlx() initializes graphics window
4. create_image() creates drawing canvas
5. render_map() renders the wireframe
   ├── get_z_range() finds min/max elevations
   ├── get_scale() calculates dynamic scale
   ├── init_render_params() sets up rendering parameters
   └── draw_connections() draws all lines
       └── draw_line() uses Bresenham algorithm
6. Event loop handles user input (ESC to close)
7. Cleanup frees all allocated memory
```

---

## Data Structures

### t_point
```c
typedef struct s_point {
    int         x;      // Column position in grid
    int         y;      // Row position in grid
    int         z;      // Elevation value
    uint32_t    color;  // Color in RGBA format
} t_point;
```
Represents a single point in the 3D map with position and color.

### t_2d_point
```c
typedef struct s_2d_point {
    int x;  // Screen X coordinate
    int y;  // Screen Y coordinate
} t_2d_point;
```
Represents a projected 2D point on screen after isometric transformation.

### t_z_range
```c
typedef struct s_z_range {
    int min;  // Minimum elevation in map
    int max;  // Maximum elevation in map
} t_z_range;
```
Stores elevation range for dynamic scaling calculations.

### t_fdf_data
```c
typedef struct s_fdf_data {
    t_point ***points;  // 3D array of point pointers
    int     rows;       // Total number of rows
    int     *cols;      // Array of column counts per row
} t_fdf_data;
```
Wrapper structure to pass map data efficiently while respecting norm limits.

---

## File-by-File Function Breakdown

### fdf.c - Map Parsing

#### `char **split_line(char *line, int *count)`
**Purpose:** Splits a line by spaces and counts the tokens.

**Parameters:**
- `line`: String to split
- `count`: Pointer to store token count

**Returns:** Array of strings (tokens)

**How it works:**
1. Calls `ft_split(line, ' ')` to split by spaces
2. Counts tokens by iterating through array until NULL
3. Updates count pointer with total tokens
4. Returns split array

**Example:**
```
Input:  "0 10,0xFF0000 5"
Output: ["0", "10,0xFF0000", "5"]  (count = 3)
```

---

#### `t_point ***allocate_points(int rows, int **cols)`
**Purpose:** Allocates memory for the 2D array of point pointers and columns array.

**Parameters:**
- `rows`: Number of rows to allocate
- `cols`: Double pointer to allocate column array

**Returns:** 
- Allocated 3D array on success
- NULL on failure

**How it works:**
1. Allocates array to store column counts: `malloc(rows * sizeof(int))`
2. If successful, allocates array of row pointers: `malloc(rows * sizeof(t_point **))`
3. Returns NULL and frees already-allocated memory on failure
4. Returns pointer to 3D array on success

**Memory structure created:**
```
points → [row0_ptr, row1_ptr, ..., rowN_ptr]
cols   → [col_count_0, col_count_1, ..., col_count_N]
```

---

#### `int count_rows(char *fdfmap)`
**Purpose:** Counts total number of rows in the map file.

**Parameters:**
- `fdfmap`: Path to .fdf file

**Returns:**
- Number of rows on success
- -1 on error

**How it works:**
1. Opens file with `open(fdfmap, O_RDONLY)`
2. Returns -1 if file cannot be opened
3. Reads each line with `get_next_line(fd)`
4. Increments counter for each line read
5. Frees each line after reading
6. Closes file descriptor
7. Returns total count

**Why it's needed:** We need to know row count before allocating memory.

---

#### `void process_line(t_point ***pts, int **cols, int row, char *line)`
**Purpose:** Processes a single line of the map file.

**Parameters:**
- `pts`: Triple pointer to points array
- `cols`: Double pointer to columns array
- `row`: Current row index
- `line`: Line string to process

**How it works:**
1. Splits line into tokens using `split_line()`
2. Stores token count in `cols[row]`
3. Allocates memory for current row: `malloc(cols[row] * sizeof(t_point *))`
4. Calls `parse_row()` to create point structures
5. Frees the split array with `free_split()`

---

#### `t_point ***parse_map(char *fdfmap, int *rows, int **cols)`
**Purpose:** Main parsing function that orchestrates the entire map reading process.

**Parameters:**
- `fdfmap`: Path to .fdf file
- `rows`: Pointer to store row count
- `cols`: Double pointer to store column counts array

**Returns:**
- 3D array of points on success
- NULL on failure

**How it works:**
1. Counts total rows with `count_rows()`
2. Returns NULL if count fails
3. Allocates memory with `allocate_points()`
4. Returns NULL if allocation fails
5. Opens file again for reading
6. Calls `read_map_lines()` to process each line
7. Closes file descriptor
8. Returns points array

**Error handling:** Returns NULL and prints error messages on any failure.

---

### fdf_utils.c - Parsing Utilities

#### `void free_points(t_point ***points, int *cols, int rows)`
**Purpose:** Frees all memory allocated for the points array.

**Parameters:**
- `points`: Triple pointer to points array
- `cols`: Array of column counts
- `rows`: Total number of rows

**How it works:**
1. Iterates through each row (i = 0 to rows-1)
2. For each row, iterates through each column (j = 0 to cols[i]-1)
3. Frees each point: `free(points[i][j])`
4. Frees the row array: `free(points[i])`
5. Frees the main points array: `free(points)`
6. Frees the columns array: `free(cols)`

**Memory deallocation order:**
```
Individual points → Row arrays → Main array → Column counts
```

---

#### `void free_split(char **split)`
**Purpose:** Frees a NULL-terminated array of strings.

**Parameters:**
- `split`: Array of strings to free

**How it works:**
1. Iterates through array until NULL terminator
2. Frees each string
3. Frees the array itself

**Usage:** Called after `ft_split()` to prevent memory leaks.

---

#### `void parse_row(t_point **row_points, char **split, int row, int col_count)`
**Purpose:** Creates point structures from split line tokens.

**Parameters:**
- `row_points`: Array to store point pointers
- `split`: Array of string tokens
- `row`: Current row index
- `col_count`: Number of columns in this row

**How it works:**
1. Iterates through each column (col = 0 to col_count-1)
2. Allocates memory for each point: `malloc(sizeof(t_point))`
3. Sets point coordinates:
   - `x = col` (column position)
   - `y = row` (row position)
   - `z = ft_atoi(split[col])` (elevation from string)
4. Parses color with `parse_color(split[col])`
5. Stores point in row_points array

**Data extraction:**
```
Token "42,0xFF0000" →
    z = 42 (via ft_atoi)
    color = 0xFF0000FF (via parse_color)
```

---

#### `int read_map_lines(t_point ***pts, int **cols, int fd, int rows)`
**Purpose:** Reads and processes all lines from the file.

**Parameters:**
- `pts`: Triple pointer to points array
- `cols`: Double pointer to columns array
- `fd`: Open file descriptor
- `rows`: Expected number of rows

**Returns:**
- 0 on success
- -1 on error

**How it works:**
1. Initializes row counter to 0
2. Reads first line with `get_next_line(fd)`
3. While line is not NULL:
   - Calls `process_line()` to parse current line
   - Frees the line string
   - Increments row counter
   - Reads next line
4. Validates that row count matches expected rows
5. Returns -1 if mismatch, 0 if successful

**Error detection:** Catches truncated or corrupted files.

---

### mlx_launch.c - Main Program

#### `int get_scale(int rows, int cols, int z_range)`
**Purpose:** Calculates dynamic scale factor based on map dimensions and elevation range.

**Parameters:**
- `rows`: Number of rows in map
- `cols`: Maximum number of columns
- `z_range`: Difference between max and min elevation

**Returns:** Scale factor (minimum 1)

**How it works:**
1. Calculates projected width: `(cols + rows) × cos(30°)`
2. Calculates projected height: `(cols + rows) × sin(30°) + z_range`
3. Calculates scale to fit 80% of window width: `scale_x = (width × 0.8) / projected_width`
4. Calculates scale to fit 80% of window height: `scale_y = (height × 0.8) / projected_height`
5. Takes minimum of scale_x and scale_y (ensures map fits in both dimensions)
6. Ensures scale is at least 1
7. Returns integer scale

**Math explanation:**
- Isometric projection stretches the map diagonally
- Width depends on both rows and columns due to diagonal projection
- Height includes both diagonal height and elevation range
- We use 80% to leave margin around the map

---

#### `int find_max_cols(int *cols, int rows)`
**Purpose:** Finds the maximum column count across all rows.

**Parameters:**
- `cols`: Array of column counts
- `rows`: Number of rows

**Returns:** Maximum column count

**How it works:**
1. Initializes max to 0
2. Iterates through all row indices
3. Updates max if current row has more columns
4. Returns maximum found

**Why needed:** Maps can have different row lengths; we need the maximum for scale calculation.

---

#### `static mlx_image_t *create_image(mlx_t *mlx)`
**Purpose:** Creates the drawable image/canvas.

**Parameters:**
- `mlx`: MLX instance

**Returns:**
- Image pointer on success
- NULL on failure (also terminates MLX)

**How it works:**
1. Calls `mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT)`
2. If creation fails:
   - Prints error message
   - Terminates MLX instance
3. Returns image pointer

---

#### `static void setup_and_run(mlx_t *mlx, mlx_image_t *img, t_fdf_data *data)`
**Purpose:** Sets up the window and starts the event loop.

**Parameters:**
- `mlx`: MLX instance
- `img`: Image to display
- `data`: Map data structure

**How it works:**
1. Displays image in window: `mlx_image_to_window(mlx, img, 0, 0)`
2. Renders the map: `render_map(img, data->points, data->rows, data->cols)`
3. Registers key handler: `mlx_key_hook(mlx, &key_hook, mlx)`
4. Registers close handler: `mlx_close_hook(mlx, &close_hook, mlx)`
5. Starts event loop: `mlx_loop(mlx)` (blocks until window closes)
6. Cleans up: `mlx_terminate(mlx)`

---

#### `int main(int argc, char **argv)`
**Purpose:** Program entry point.

**Parameters:**
- `argc`: Argument count
- `argv`: Argument values

**Returns:**
- 0 on success
- 1 on error

**How it works:**
1. Validates argument count (must be exactly 2: program name + map file)
2. Parses map file with `parse_map()`
3. Returns 1 if parsing fails
4. Initializes MLX with `init_mlx()`
5. Frees points and returns 1 if MLX init fails
6. Enables image stretching: `mlx_set_setting(MLX_STRETCH_IMAGE, true)`
7. Creates image with `create_image()`
8. Frees points and returns 1 if image creation fails
9. Calls `setup_and_run()` to render and run event loop
10. Frees points after window closes
11. Returns 0

**Memory management:** Always frees allocated points before returning.

---

### render.c - Rendering Engine

#### `static t_z_range get_z_range(t_point ***pts, int rows, int *cols)`
**Purpose:** Finds minimum and maximum elevation values in the map.

**Parameters:**
- `pts`: 3D array of points
- `rows`: Number of rows
- `cols`: Array of column counts

**Returns:** Structure containing min and max z values

**How it works:**
1. Initializes both min and max to first point's z value
2. Iterates through all rows
3. For each row, iterates through all columns
4. Updates min if current z is smaller
5. Updates max if current z is larger
6. Returns range structure

**Why needed:** Essential for proper scaling and centering of elevated maps.

---

#### `mlx_t *init_mlx(void)`
**Purpose:** Initializes the MLX graphics library.

**Parameters:** None

**Returns:**
- MLX instance on success
- NULL on failure

**How it works:**
1. Calls `mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true)`
2. Prints error if initialization fails
3. Returns MLX pointer (or NULL)

---

#### `void draw_connections(mlx_image_t *img, t_point ***pts, int *p, int *cols)`
**Purpose:** Draws lines connecting a point to its neighbors (right and down).

**Parameters:**
- `img`: Image to draw on
- `pts`: 3D array of points
- `p`: Array containing [row, col, scale, offset_x, offset_y, total_rows]
- `cols`: Array of column counts

**How it works:**
1. Projects current point to 2D: `project_and_scale(pts[row][col], scale)`
2. Adds offsets to center the map: `p1.x += offset_x`, `p1.y += offset_y`
3. If not at last column of row:
   - Projects right neighbor point
   - Adds offsets
   - Draws line from current to right neighbor
4. If not at last row AND next row has this column:
   - Projects down neighbor point
   - Adds offsets
   - Draws line from current to down neighbor

**Connection pattern:**
```
[current] ─→ [right]
    |
    ↓
  [down]
```
This creates a complete wireframe mesh by connecting each point to its right and bottom neighbors.

---

#### `static void init_render_params(int *params, t_point ***pts, int rows, int *cols)`
**Purpose:** Calculates all rendering parameters (scale and offsets) in one function.

**Parameters:**
- `params`: Array to store [row, col, scale, offset_x, offset_y, total_rows]
- `pts`: 3D array of points
- `rows`: Number of rows
- `cols`: Array of column counts

**How it works:**
1. Finds maximum column count
2. Gets z range (min/max elevations)
3. Calculates scale using `get_scale(rows, max_cols, z_range.max - z_range.min)`
4. Calculates projected map width:
   - `(max_cols - 1 + rows - 1) × cos(30°) × scale`
   - Diagonal width accounting for both dimensions
5. Calculates projected map height:
   - `(max_cols - 1 + rows - 1) × sin(30°) × scale + elevation_range × scale`
   - Diagonal height plus elevation offset
6. Calculates horizontal offset:
   - `(WINDOW_WIDTH - map_width) / 2 + (rows - 1) × cos(30°) × scale`
   - Centers map horizontally, accounts for isometric left shift
7. Calculates vertical offset:
   - `(WINDOW_HEIGHT - map_height) / 2 + z_max × scale`
   - Centers map vertically, accounts for maximum elevation
8. Stores all parameters in array

**Parameters array:**
- params[0]: row (set by caller)
- params[1]: col (set by caller)
- params[2]: scale
- params[3]: offset_x
- params[4]: offset_y
- params[5]: total_rows

---

#### `void render_map(mlx_image_t *img, t_point ***points, int rows, int *cols)`
**Purpose:** Main rendering function that draws the entire wireframe.

**Parameters:**
- `img`: Image to draw on
- `points`: 3D array of points
- `rows`: Number of rows
- `cols`: Array of column counts

**How it works:**
1. Clears image to black background
2. Returns early if map is empty
3. Calls `init_render_params()` to calculate scale and offsets
4. Nested loop through all rows and columns:
   - Sets current row/col in params array
   - Calls `draw_connections()` to draw lines from current point

**Result:** Complete wireframe mesh rendered on screen.

---

### draw_utils.c - Drawing Primitives

#### `t_2d_point project_and_scale(t_point *point, int scale)`
**Purpose:** Transforms 3D point to 2D isometric projection and scales it.

**Parameters:**
- `point`: 3D point with x, y, z coordinates
- `scale`: Scaling factor

**Returns:** 2D projected point

**How it works:**
1. Applies isometric projection formulas:
   - `x_2d = (x - y) × cos(30°) × scale`
   - `y_2d = (x + y) × sin(30°) × scale - z × scale`
2. Casts results to integers
3. Returns 2D point structure

**Isometric projection explanation:**
- Standard isometric uses 30° angle (also called "dimetric")
- X difference creates horizontal spread
- X+Y sum creates diagonal depth
- Z value raises/lowers the point vertically
- Cos(30°) ≈ 0.866 and Sin(30°) = 0.5 create the characteristic isometric look

**Visual:**
```
3D Grid (x,y,z)          2D Screen
    ↓                       ↓
(1,0,0) ──→  shifted right by cos(30°)
(0,1,0) ──→  shifted left by cos(30°)
(0,0,1) ──→  shifted up
```

---

#### `void step_x(t_2d_point *p1, t_2d_point p2, int *err, int dy)`
**Purpose:** Advances along X axis in Bresenham algorithm.

**Parameters:**
- `p1`: Current point (modified)
- `p2`: Target point
- `err`: Error accumulator (modified)
- `dy`: Absolute difference in Y

**How it works:**
1. Decrements error by dy
2. Increments x if p1.x < p2.x
3. Decrements x if p1.x > p2.x

**Why needed:** Bresenham needs to track which direction to move.

---

#### `void step_y(t_2d_point *p1, t_2d_point p2, int *err, int dx)`
**Purpose:** Advances along Y axis in Bresenham algorithm.

**Parameters:**
- `p1`: Current point (modified)
- `p2`: Target point
- `err`: Error accumulator (modified)
- `dx`: Absolute difference in X

**How it works:**
1. Increments error by dx
2. Increments y if p1.y < p2.y
3. Decrements y if p1.y > p2.y

---

#### `void draw_line(mlx_image_t *img, t_2d_point p1, t_2d_point p2, uint32_t color)`
**Purpose:** Draws a line between two points using Bresenham's algorithm.

**Parameters:**
- `img`: Image to draw on
- `p1`: Start point
- `p2`: End point
- `color`: RGBA color value

**How it works:**
1. Calculates absolute differences: `dx = abs(p2.x - p1.x)`, `dy = abs(p2.y - p1.y)`
2. Initializes error: `err = dx - dy`
3. Infinite loop:
   - If current point is within image bounds, draws pixel
   - If reached target point (p1 == p2), breaks
   - Calculates error doubled: `e2 = err * 2`
   - If `e2 > -dy`, steps along X axis
   - If `e2 < dx`, steps along Y axis
4. Continues until line is complete

**Bresenham's algorithm:**
- Most efficient line drawing algorithm
- Uses only integer arithmetic (no floating point)
- Error accumulation determines when to step in each direction
- Produces smooth, connected lines even for diagonal slopes

**Boundary checking:** Prevents drawing outside image, avoiding crashes.

---

#### `void clear_image(mlx_image_t *img)`
**Purpose:** Fills entire image with black color.

**Parameters:**
- `img`: Image to clear

**How it works:**
1. Nested loop through all pixels (y = 0 to height-1, x = 0 to width-1)
2. Sets each pixel to black: `mlx_put_pixel(img, x, y, 0x000000FF)`
   - 0x000000 = RGB black
   - 0xFF = fully opaque alpha channel

**When used:** Called at start of `render_map()` to clear previous frame.

---

### color_utils.c - Color Parsing

#### `static int ft_hexdigit(char c)`
**Purpose:** Converts a single hexadecimal character to its integer value.

**Parameters:**
- `c`: Character ('0'-'9', 'a'-'f', 'A'-'F')

**Returns:** Integer value (0-15)

**How it works:**
1. If '0'-'9': returns (c - '0') → values 0-9
2. If 'a'-'f': returns (c - 'a' + 10) → values 10-15
3. If 'A'-'F': returns (c - 'A' + 10) → values 10-15
4. Otherwise returns 0 (invalid character)

**Examples:**
- '7' → 7
- 'A' → 10
- 'f' → 15

---

#### `static uint32_t parse_hex_color(char *str)`
**Purpose:** Converts hexadecimal color string to 32-bit RGBA value.

**Parameters:**
- `str`: Hex color string (e.g., "FF0000" for red)

**Returns:** RGBA color value

**How it works:**
1. Initializes result to 0
2. Processes up to 8 characters:
   - Multiplies result by 16 (left shift in hex)
   - Adds next hex digit value
3. Left shifts result by 8 bits: `result << 8`
4. Adds alpha channel: `| 0xFF` (fully opaque)
5. Returns final RGBA value

**Example transformation:**
```
Input:  "FF0000"
Step 1: result = 0xF
Step 2: result = 0xFF
Step 3: result = 0xFF0
Step 4: result = 0xFF00
Step 5: result = 0xFF000
Step 6: result = 0xFF0000
Final:  0xFF0000FF (left shift 8 + add FF)
```

**Color format:** 0xRRGGBBAA (Red, Green, Blue, Alpha)

---

#### `uint32_t parse_color(char *str)`
**Purpose:** Parses color from map token, defaulting to white if no color specified.

**Parameters:**
- `str`: Token string (e.g., "42" or "42,0xFF0000")

**Returns:** RGBA color value

**How it works:**
1. Searches for comma in string: `ft_strchr(str, ',')`
2. If comma found:
   - Checks if followed by "0x" (hex prefix)
   - If yes, parses hex color starting after "0x"
3. If no valid color format found, returns white (0xFFFFFFFF)

**Map file format:**
- `42` → elevation 42, white color
- `42,0xFF0000` → elevation 42, red color

---

### hooks.c - Event Handling

#### `void key_hook(mlx_key_data_t keydata, void *param)`
**Purpose:** Handles keyboard input events.

**Parameters:**
- `keydata`: Key event data (which key, action type)
- `param`: User parameter (MLX instance)

**How it works:**
1. Casts param to mlx_t pointer
2. Checks if ESC key was pressed: `keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS`
3. If yes, closes window: `mlx_close_window(mlx)`

**Event types:** MLX_PRESS (key pressed), MLX_RELEASE (key released), MLX_REPEAT (key held)

---

#### `void close_hook(void *param)`
**Purpose:** Handles window close event (clicking X button).

**Parameters:**
- `param`: User parameter (MLX instance)

**How it works:**
1. Casts param to mlx_t pointer
2. Closes window: `mlx_close_window(mlx)`

**When triggered:** User clicks window's close button (X).

---

## Algorithm Explanations

### Isometric Projection

The program converts 3D coordinates (x, y, z) to 2D screen coordinates using isometric projection:

**Formulas:**
```
screen_x = (x - y) × cos(30°) × scale
screen_y = (x + y) × sin(30°) × scale - z × scale
```

**Why these formulas:**
1. `(x - y)` creates horizontal spread (columns go right, rows go left)
2. `(x + y)` creates depth (both columns and rows go down-right)
3. `× cos(30°)` and `× sin(30°)` create the 30° angle characteristic
4. `-z` elevates high points and lowers low points
5. `× scale` makes everything visible at appropriate size

**Visual representation:**
```
     (0,1)
      /|
     / |
(0,0)  |  (1,0)
    \  | /
     \ |/
     (0,0) projected
```

### Bresenham's Line Algorithm

Draws straight lines using only integer arithmetic:

**Core concept:**
- Error accumulation determines when to step in each direction
- Tracks deviation from ideal line
- Always moves along the major axis
- Conditionally moves along minor axis

**Why it's optimal:**
- No floating-point calculations (much faster)
- No multiplication/division in inner loop
- Produces smooth, connected pixels
- Guaranteed to never skip pixels

### Dynamic Scaling

Automatically calculates scale to fit any map size:

**Process:**
1. Calculate how wide the map would be in projected space
2. Calculate how tall it would be (including elevation)
3. Find scale that fits width in 80% of window width
4. Find scale that fits height in 80% of window height
5. Use smaller scale (ensures both fit)

**Why 80%:** Leaves 10% margin on each side for visual appeal.

### Centering with Elevation

Centers the map while accounting for highest point:

**Horizontal centering:**
```
offset_x = (window_width - projected_width) / 2 + isometric_shift
```

**Vertical centering:**
```
offset_y = (window_height - projected_height) / 2 + max_elevation
```

The max_elevation offset prevents high points from being cut off at the top.

---

## Summary

This FdF implementation demonstrates:
- **Robust parsing** with error handling at every step
- **Dynamic rendering** that adapts to any map size
- **Efficient algorithms** (Bresenham for drawing, isometric projection)
- **Clean architecture** with single-responsibility functions
- **Memory safety** with comprehensive cleanup
- **42 Norm compliance** with helper functions and parameter reduction

The program successfully transforms elevation data into visually appealing 3D wireframe representations using classic computer graphics techniques.
