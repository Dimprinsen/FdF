/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:24 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 17:42:22 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*ptr2;
	size_t			i;

	ptr = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (ptr[i] != ptr2[i])
			return (ptr[i] - ptr2[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	char	str1[] = "abcdef";
	char	str2[] = "abcdez";
	char	str3[] = "abcdef";

	// Test 1: Equal strings
	printf("Test 1: Equal strings\n");
	printf("ft_memcmp: %d\n", ft_memcmp(str1, str3, 6)); // Expected: 0

	// Test 2: Difference at last byte
	printf("Test 2: Difference at last byte\n");
	printf("ft_memcmp: %d\n", ft_memcmp(str1, str2, 6)); // Expected: negative value

	// Test 3: Compare only first 3 bytes (equal)
	printf("Test 3: First 3 bytes are equal\n");
	printf("ft_memcmp: %d\n", ft_memcmp(str1, str2, 3)); // Expected: 0

	// Test 4: Zero-length comparison
	printf("Test 4: Zero-length comparison\n");
	printf("ft_memcmp: %d\n", ft_memcmp(str1, str2, 0)); // Expected: 0

	return (0);
}//*/
