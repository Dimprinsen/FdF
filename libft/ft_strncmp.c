/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:46:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 10:48:31 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*#include <stdio.h>
int	main(void)
{
	printf("Test 1: ");
	printf("ft_strncmp: %d\n strncmp: %d\n", ft_strncmp("abc", "abc", 3));
	printf("strncmp: %d\n", strncmp("abc", "abc", 3));
	printf("Test 2: ");
	printf("ft_strncmp: %d\n", ft_strncmp("abc", "abd", 3));
	printf("ft_strncmp: %d", ft_strncmp("abc", "abd", 3));
	printf("Test 3: ");
	printf("ft_strncmp: %d ft_strncmp("abc", "abcde", 5))
	printf("strncmp: %d\n", strncmp("abc", "abcde", 5));
	printf("Test 5: ");
	printf("ft_strncmp: %d\n", ft_strncmp("abc", "", 2));
	printf("strncmp: %d\n", strncmp("abc", "", 2));
	return (0);
}//*/