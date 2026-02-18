/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2026/02/15 17:43:49 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	ft_hexdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

static uint32_t	parse_hex_color(char *str)
{
	uint32_t	result;
	int			i;

	result = 0;
	i = 0;
	while (str[i] && i < 8)
	{
		result = result * 16 + ft_hexdigit(str[i]);
		i++;
	}
	return ((result << 8) | 0xFF);
}

uint32_t	parse_color(char *str)
{
	char	*comma;

	comma = ft_strchr(str, ',');
	if (comma && *(comma + 1) == '0' && *(comma + 2) == 'x')
		return (parse_hex_color(comma + 3));
	return (0x40E0D0FF);
}
