/* ************************************************************************** */
/* Annotated copy of color_utils.c — hex parsing and color format explained */
/* ************************************************************************** */

#include "fdf.h"

/*
 * ft_hexdigit
 * - Convert a single hex character to its numeric value (0..15).
 */
static int	ft_hexdigit(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    return (0); // non-hex fallback
}

/*
 * parse_hex_color
 * - Read up to 8 hex digits and build a 32-bit color value.
 * - The function shifts the parsed value left 8 bits and sets the low
 *   byte to 0xFF to ensure alpha is opaque. Result layout: 0xRRGGBBAA
 */
static uint32_t	parse_hex_color(char *str)
{
    uint32_t	result;
    int		i;

    result = 0;
    i = 0;
    while (str[i] && i < 8)
    {
        result = result * 16 + ft_hexdigit(str[i]); // accumulate hex
        i++;
    }
    return ((result << 8) | 0xFF); // shift and force alpha=0xFF
}

/*
 * parse_color
 * - If token contains ",0x..." parse the hex color; otherwise return
 *   a default color (here teal-ish with full alpha).
 */
uint32_t	parse_color(char *str)
{
    char	*comma;

    comma = ft_strchr(str, ','); // look for comma separating height and color
    if (comma && *(comma + 1) == '0' && *(comma + 2) == 'x')
        return (parse_hex_color(comma + 3)); // parse after "0x"
    return (0x40E0D0FF); // default color (0xRRGGBBAA)
}
