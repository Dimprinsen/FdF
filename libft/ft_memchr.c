/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:18 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 17:41:50 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char) c)
			return (&ptr[i]);
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
int	main(void)
{
	const char str[] = "abcdef";

	printf("Test 1: Looking for 'd' in \"%s\" (n=6):\n", str);
	char *result = ft_memchr(str, 'd', 6);
	if (result)
		printf("Found: %s\n", result); // expected: "def"
	else
		printf("Not found\n");

	printf("Test 2: Looking for 'z' (n=6):\n");
	result = ft_memchr(str, 'z', 6);
	if (result)
		printf("Found: %s\n", result);
	else
		printf("Not found\n"); // expected
}//*/