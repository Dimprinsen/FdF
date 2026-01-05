/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:57:11 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:51:05 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n > 0)
	{
		*ptr = (unsigned char)c;
		ptr++;
		n--;
	}
	return (s);
}
/*
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
int	main()
{
	char buf1[10];
	char buf2[10];

	ft_memset(buf1, 'X', sizeof(buf1));
	memset(buf2, 'X', sizeof(buf2));

	size_t i = 0;
	
	while (i < sizeof(buf1))
	{
		printf("%c", buf1[i]);
		i++;
	}
	printf("\n");

	size_t j = 0;

	while (j < sizeof(buf2))
	{
		printf("%c", buf2[i]);
		j++;
	}
	printf("\n");
}
*/
