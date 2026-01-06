# FdF - Evaluation Guide

## Table of Contents
1. [Program Overview](#program-overview)
2. [Key Concepts](#key-concepts)
3. [Program Flow](#program-flow)
4. [Critical Functions](#critical-functions)
5. [MLX42 Functions](#mlx42-functions)
6. [Common Evaluation Questions](#common-evaluation-questions)
7. [Demonstration](#demonstration)

---

## Program Overview

**FdF** (Fil de Fer / Wireframe) is a 3D wireframe viewer that displays height maps in isometric projection.

**What it does:**
- Reads a `.fdf` file containing a grid of height values
- Projects 3D coordinates to 2D screen using isometric projection
- Draws lines between grid points using Bresenham's algorithm
- Displays the result in a window with ESC to exit

**Key technologies:**
- MLX42 graphics library
- Isometric projection mathematics
- Bresenham's line drawing algorithm

---

## Key Concepts

### Isometric Projection

Converts 3D coordinates (x, y, z) to 2D screen coordinates:

```c
// Formula in project_and_scale():
screen_x = (x - y) * cos(30°) * scale
screen_y = (x + y) * sin(30°) * scale - z * scale
```

**Why these formulas?**
- `cos(30°) ≈ 0.866` creates the 30° angle
- `sin(30°) ≈ 0.5` creates the horizontal compression
- Subtracting z makes higher points appear higher on screen (y decreases upward)

### Bresenham's Line Algorithm

Fast integer-only line drawing algorithm.

**Why Bresenham?**
- No floating point math (faster)
- Only uses addition, subtraction, and bit shifts
- Produces smooth, accurate lines

**How it works:**
- Error accumulator decides when to step in x or y
- `err = dx - dy` (initial error)
- Each step adjusts error to keep line straight

---

## Program Flow

### Complete Execution Order

```
main()
├─ parse_map()              ← Read and parse .fdf file
│  ├─ count_rows()         ← Count lines in file
│  ├─ allocate_points()    ← Allocate memory
│  ├─ read_map_lines()     ← Read file again
│  │  └─ process_line()    ← Process each line
│  │     ├─ split_line()  ← Split by spaces, count columns
│  │     └─ parse_row()   ← Create point structs
│  │        └─ parse_color() ← Parse hex colors
│  │           └─ parse_hex_color()
│  │              └─ ft_hexdigit()
│
├─ init_mlx()              ← Create window
├─ create_image()          ← Create pixel buffer
│
└─ setup_and_run()         ← Render and event loop
   ├─ mlx_image_to_window() ← Display image
   ├─ render_map()         ← Draw wireframe
   │  ├─ clear_image()    ← Black background
   │  ├─ init_render_params() ← Calculate scale/offsets
   │  │  ├─ find_max_cols()
   │  │  ├─ get_z_range() ← Find min/max height
   │  │  └─ get_scale()   ← Calculate scale factor
   │  └─ draw_connections() ← Draw all lines
   │     ├─ project_and_scale() ← 3D → 2D projection
   │     └─ draw_line()   ← Bresenham's algorithm
   │        ├─ step_x()
   │        ├─ step_y()
   │        └─ put_pixel() ← Set pixel color
   │
   ├─ mlx_key_hook()       ← Register keyboard handler
   ├─ mlx_close_hook()     ← Register close handler
   ├─ mlx_loop()           ← Event loop (BLOCKS)
   └─ mlx_terminate()      ← Cleanup
```

---

## Critical Functions

### 1. parse_map() - Entry Point for Parsing

**File:** `fdf.c`

**Purpose:** Orchestrates reading and parsing the .fdf file

**How it works:**
1. Counts rows (first pass through file)
2. Allocates memory for points array
3. Opens file again
4. Reads and processes each line
5. Returns 3D point array

**Key variables:**
- `t_point ***points` - 3D array: `points[row][col] → t_point`
- `int *rows` - Output: number of rows
- `int **cols` - Output: array of column counts per row

**Why two passes?**
- First pass: count rows to allocate correct memory
- Second pass: actually parse the data

---

### 2. project_and_scale() - The Math

**File:** `draw_utils.c`

**Purpose:** Converts 3D grid coordinates to 2D screen position

**The isometric projection formulas:**
```c
projected.x = (point->x - point->y) * COS_30 * scale;
projected.y = (point->x + point->y) * SIN_30 * scale - point->z * scale;
```

**Example:**
- Input: point(5, 3, 2), scale=20
- Output: screen coordinates (34, 40)

**Why these specific formulas?**
- Creates the characteristic isometric "diamond" view
- 30° angle is standard for isometric projection
- z subtraction makes height go upward on screen

---

### 3. draw_line() - Bresenham's Algorithm

**File:** `draw_utils.c`

**Purpose:** Draws a line from p1 to p2 using Bresenham's algorithm

**The algorithm:**
```c
dx = abs(p2.x - p1.x);     // Horizontal distance
dy = abs(p2.y - p1.y);     // Vertical distance
err = dx - dy;              // Error accumulator

while (true) {
    put_pixel(img, p1.x, p1.y, color);
    if (reached endpoint) break;
    
    e2 = err * 2;
    if (e2 > -dy) step_x();  // Move in x
    if (e2 < dx)  step_y();  // Move in y
}
```

**Key concept - Error accumulator:**
- Decides when to step in x vs y direction
- Keeps line as straight as possible
- Uses only integer math (fast!)

**Why Bresenham?**
- Fast: no floating point, no division
- Accurate: minimal error
- Industry standard since 1962

---

### 4. render_map() - Drawing Orchestrator

**File:** `render.c`

**Purpose:** Coordinates the entire rendering process

**Steps:**
1. Clear image (black background)
2. Calculate rendering parameters (scale, offsets)
3. Loop through every point
4. For each point, draw lines to right and down neighbors

**Why only right and down?**
- Each point connects to right neighbor (horizontal line)
- Each point connects to down neighbor (vertical line)
- This creates complete grid without duplicate lines

---

### 5. put_pixel() - Lowest Level Drawing

**File:** `draw_utils.c`

**Purpose:** Sets a single pixel's color in the image buffer

**How it works:**
```c
// 1. Calculate byte position
index = (y * img->width + x) * 4;  // 4 bytes per pixel (RGBA)

// 2. Get pointer to pixel
pixel = &img->pixels[index];

// 3. Write RGBA bytes
*(pixel++) = (uint8_t)(color >> 24);  // Red
*(pixel++) = (uint8_t)(color >> 16);  // Green
*(pixel++) = (uint8_t)(color >> 8);   // Blue
*(pixel++) = (uint8_t)(color & 0xFF); // Alpha
```

**Why bounds check?**
```c
if (x >= 0 && x < img->width && y >= 0 && y < img->height)
```
- Prevents crashes from drawing outside image
- Lines can extend beyond window during projection

---

## MLX42 Functions

### Window Management

#### `mlx_init(width, height, title, resizable)`
**File:** Called in `init_mlx()`

**Purpose:** Creates the window and graphics context

**Parameters:**
- `width`, `height` - Window size (1024 x 640)
- `title` - Window title ("FdF")
- `resizable` - true/false

**Returns:** `mlx_t*` - MLX instance or NULL on error

**Example:**
```c
mlx = mlx_init(1024, 640, "FdF", true);
```

---

#### `mlx_terminate(mlx)`
**File:** Called in `setup_and_run()`

**Purpose:** Frees all MLX resources and closes window

**When:** After event loop exits

---

### Image Functions

#### `mlx_new_image(mlx, width, height)`
**File:** Called in `create_image()`

**Purpose:** Creates a pixel buffer (blank canvas)

**Parameters:**
- `mlx` - The MLX instance
- `width`, `height` - Image size in pixels

**Returns:** `mlx_image_t*` - Image buffer

**What it allocates:**
- Array of pixels: `width * height * 4` bytes
- Each pixel = RGBA (4 bytes)

**Example:**
```c
img = mlx_new_image(mlx, 1024, 640);  // 2,621,440 bytes
```

---

#### `mlx_image_to_window(mlx, img, x, y)`
**File:** Called in `setup_and_run()`

**Purpose:** Displays the image in the window

**Parameters:**
- `mlx` - MLX instance
- `img` - Image to display
- `x`, `y` - Position in window (0, 0 = top-left)

**When:** After creating image, before rendering

---

### Event Hooks

#### `mlx_key_hook(mlx, function, param)`
**File:** Called in `setup_and_run()`

**Purpose:** Registers keyboard event callback

**Parameters:**
- `mlx` - MLX instance
- `function` - Function to call: `void func(mlx_key_data_t, void*)`
- `param` - User data to pass to function

**Example:**
```c
mlx_key_hook(mlx, &key_hook, mlx);

// In key_hook:
void key_hook(mlx_key_data_t keydata, void *param) {
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window((mlx_t*)param);
}
```

**Key codes:**
- `MLX_KEY_ESCAPE` - ESC key
- `MLX_KEY_W`, `MLX_KEY_A`, etc.

**Actions:**
- `MLX_PRESS` - Key pressed down
- `MLX_RELEASE` - Key released
- `MLX_REPEAT` - Key held (auto-repeat)

---

#### `mlx_close_hook(mlx, function, param)`
**File:** Called in `setup_and_run()`

**Purpose:** Registers window close callback (X button)

**Parameters:**
- `mlx` - MLX instance
- `function` - Function to call: `void func(void*)`
- `param` - User data

---

### Event Loop

#### `mlx_loop(mlx)`
**File:** Called in `setup_and_run()`

**Purpose:** Main event loop - handles events and rendering

**Important:** **BLOCKS** until window closes
- Continuously checks for events
- Calls registered hooks when events occur
- Updates window display
- Only returns when `mlx_close_window()` is called

---

#### `mlx_close_window(mlx)`
**File:** Called in hooks

**Purpose:** Signals the event loop to exit

**When:** Called from key_hook (ESC) or close_hook (X button)

---

### Utility

#### `mlx_set_setting(setting, value)`
**File:** (Removed from current version)

**Purpose:** Configure MLX behavior

**Example:**
```c
mlx_set_setting(MLX_STRETCH_IMAGE, true);  // Auto-scale image
```

**Note:** Not required for basic functionality

---

## Common Evaluation Questions

### 1. "Explain isometric projection"

**Answer:**
"Isometric projection converts 3D coordinates to 2D by using specific angles. The formulas are:
- x_screen = (x - y) × cos(30°) × scale
- y_screen = (x + y) × sin(30°) × scale - z × scale

This creates a 30° angle view where parallel lines stay parallel. Height (z) is subtracted because higher values should appear higher on screen, and y decreases upward in screen coordinates."

---

### 2. "Why Bresenham's algorithm?"

**Answer:**
"Bresenham's algorithm is the standard for line drawing because:
1. **Fast** - Uses only integer math (addition, subtraction, bit shifts)
2. **Accurate** - Produces smooth lines with minimal error
3. **Simple** - Error accumulator decides when to step in x or y
4. **No division or floating point** - Makes it very efficient

The error term (dx - dy) is adjusted each step to keep the line as straight as possible."

---

### 3. "Walk me through the program execution"

**Answer:**
"The program follows these steps:

1. **Parse** - Read .fdf file, convert to 3D point array
2. **Initialize** - Create MLX window and image buffer
3. **Calculate** - Determine scale and centering offsets
4. **Render** - Loop through all points:
   - Project each 3D point to 2D
   - Draw lines to right and down neighbors
   - Use Bresenham for smooth lines
5. **Event loop** - Wait for ESC or window close
6. **Cleanup** - Free memory and terminate MLX"

---

### 4. "How do you handle different map sizes?"

**Answer:**
"The program dynamically scales based on map size:

1. **Find dimensions** - Get max columns and z-range
2. **Calculate projected size** - Apply isometric formulas
3. **Scale to fit** - Use 90% of window size:
   - `scale = min(window_width / projected_width, window_height / projected_height)`
4. **Center** - Calculate offsets to center the map
5. **Minimum scale** - Never go below 1 pixel per unit

This ensures any map fits in the window while using maximum available space."

---

### 5. "Explain your data structure"

**Answer:**
"I use a 3D array structure: `t_point ***points`

- **First pointer** - Array of rows
- **Second pointer** - Array of columns (per row)
- **Third pointer** - Actual t_point struct

Example: `points[row][col]->z` accesses the height.

**Why this structure?**
- Handles irregular maps (different column counts per row)
- Dynamic allocation (size determined at runtime)
- Clean syntax for accessing points

Each t_point contains: x, y, z (grid coords) and color."

---

### 6. "What's the most challenging part?"

**Answer:**
"The scaling and centering logic in `init_render_params()`. You need to:

1. Calculate projected dimensions using isometric formulas
2. Account for z-range (height variation)
3. Find the right scale factor to fit window
4. Calculate x and y offsets to center the map
5. Handle edge cases (very tall/flat maps)

Getting this right ensures all maps display properly centered and sized."

---

### 7. "Show me memory management"

**Answer:**
"The program has careful memory management:

**Allocation:**
- Parse phase: malloc for points array, cols array, each point
- MLX creates: window, image buffer (automatic)

**Freeing:**
- `free_points()` - Triple nested loop:
  - Free each point struct
  - Free each row array
  - Free points array
  - Free cols array
- MLX cleanup: `mlx_terminate()` frees window/image

**No leaks:**
- Every malloc has matching free
- Valgrind shows 0 definitely lost bytes
- MLX library leaks are suppressed (not our code)"

---

### 8. "What are the MLX42 types?"

**Answer:**
"`mlx_t*` - The MLX instance (window + context)
`mlx_image_t*` - Image buffer with pixel array
`mlx_key_data_t` - Keyboard event data
`uint32_t` - Not from MLX! Standard C type for colors

MLX provides these as opaque types - we use them but don't see their internal structure. They're defined in MLX42.h."

---

## Demonstration

### Basic Demo
```bash
# Compile
make

# Run with different maps
./fdf test_maps/42.fdf
./fdf test_maps/t1.fdf
./fdf test_maps/mars.fdf

# Press ESC to close
```

### Memory Check
```bash
# With suppression file
valgrind --leak-check=full --suppressions=valgrind.supp ./fdf test_maps/42.fdf

# Expected output:
# definitely lost: 0 bytes in 0 blocks
# ERROR SUMMARY: 0 errors from 0 contexts
```

### Scaling Demonstration
```bash
# Small map - should be large on screen
./fdf test_maps/t1.fdf

# Large map - should be scaled down
./fdf test_maps/mars.fdf

# All maps should be centered and fit in window
```

### Color Support
```bash
# Maps with embedded colors
./fdf test_maps/elem-col.fdf   # Has 0xRRGGBB colors
./fdf test_maps/42.fdf          # Default turquoise (0x40E0D0FF)
```

---

## Quick Reference

### Important Constants
```c
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 640
#define COS_30 0.86602540378
#define SIN_30 0.5
```

### Key Structs
```c
typedef struct s_point {
    int x, y, z;        // Grid coordinates
    uint32_t color;     // RGBA color
} t_point;

typedef struct s_2d_point {
    int x, y;           // Screen coordinates
} t_2d_point;

typedef struct s_map_data {
    t_point ***points;  // 3D point array
    int rows;           // Number of rows
    int *cols;          // Columns per row
} t_map_data;
```

### File Organization (5 functions max per file)
- **fdf.c** - Parsing orchestration
- **fdf_utils.c** - Parsing helpers
- **render.c** - Rendering orchestration
- **draw_utils.c** - Drawing functions
- **mlx_launch.c** - MLX setup and main
- **hooks.c** - Event handlers
- **color_utils.c** - Color parsing

---

## Tips for Evaluation

1. **Run the program first** - Show it working with various maps
2. **Explain high-level first** - Overview before details
3. **Use the code** - Point to actual functions while explaining
4. **Know Bresenham** - Very likely to be asked
5. **Know isometric math** - Explain the projection formulas
6. **Memory check** - Run valgrind to show no leaks
7. **Be honest** - If you don't know something, say so and find it together

**Good luck with your evaluation!** 🚀
