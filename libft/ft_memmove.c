/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:10 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 13:24:02 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*ptr2;

	ptr = (unsigned char *) dest;
	ptr2 = (unsigned char *) src;
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	if (dest > src)
		while (n--)
			ptr[n] = ptr2[n];
	return (dest);
}
/*
#include <stdio.h>
int main(void)
{
    char str1[20] = "Hello, world!";
    char str2[20] = "Hello, world!";
	char str3[20] = "1234567890";

    // Test 1: Non-overlapping copy (should behave like memcpy)
    ft_memmove(str1, "Goodbye", 8);
    printf("Test 1: %s\n", str1);

    // Test 2: Overlapping copy, dest < src (forward copy)
    ft_memmove(str2, str2 + 7, 6);  // Move "world!" to start
    printf("Test 2: %s\n", str2);

    // Test 3: Overlapping copy, dest > src (backward copy)
    ft_memmove(str3 + 4, str3, 6);  // Move "123456" into starting at str3[4]
    printf("Test 3: %s\n", str3);

    return 0;
}//*/