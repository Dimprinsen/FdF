/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 12:58:11 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 13:12:08 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (size - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
/*#include <stdio.h>
#include <bsd/string.h>
int	main(void)
{
	char	dest1[20] = "Hello";
	char	dest2[20] = "Hello";
	const char *src = " World!";
	size_t size = sizeof(dest1);

	size_t std_ret = strlcat(dest1, src, size);
	size_t ft_ret = ft_strlcat(dest2, src, size);

	printf("Standard strlcat result: \"%s\" (ret = %zu)\n", dest1, std_ret);
	printf("Your     ft_strlcat result: \"%s\" (ret = %zu)\n", dest2, ft_ret);

	if (strcmp(dest1, dest2) == 0 && std_ret == ft_ret)
		printf("✅ Match!\n");
	else
		printf("❌ Mismatch!\n");

	return (0);
}
//*/
