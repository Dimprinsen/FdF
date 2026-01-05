/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 12:37:11 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 12:47:27 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	srclen;
	size_t	i;

	i = 0;
	srclen = ft_strlen(src);
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srclen);
}
/*
#include <bsd/string.h>
#include <stdio.h>
int     main()
{
        char src[] = "Hello there!";
        char dest[6];
	unsigned int len = ft_strlcpy(dest, src, 6);
        printf("Copied string: %s Expected: Hello  Length: %u\n", dest, len);
	char src1[] = "Hello there!";
        char dest1[6];
        unsigned int len1 = strlcpy(dest1, src1, 6);
        printf("Copied string: %s Expected: Hello  Length: %u\n", dest1, len1);

	return (0);
}
//*/
