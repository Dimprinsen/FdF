/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 12:34:55 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/16 15:19:59 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}
/*
#include <stdio.h>
int	main(void)
{
    char *str = "hello world";

    char *result = ft_strchr(str, 'a');
    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not found\n");

    result = ft_strchr(str, 'z');
    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not found\n");

    return 0;
}
//*/
