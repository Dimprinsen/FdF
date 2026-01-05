/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:46:17 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 11:23:21 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = ft_strlen(s);
	if (!s)
		return (NULL);
	if ((unsigned char)c == 0)
		return ((char *)s + i);
	while (i > 0)
	{
		if (s[i - 1] == (unsigned char)c)
			return ((char *)s + i - 1);
		i--;
	}
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
