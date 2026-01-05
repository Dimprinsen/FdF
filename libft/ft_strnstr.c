/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 12:11:51 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 12:31:00 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*little)
		return ((char *)big);
	i = 0;
	j = 0;
	while (big[i] && i < len)
	{
		if (big[i] == little[0])
		{
			while (big[i + j] == little[j] && little[j] && i + j < len)
			{
				if (little[j + 1] == '\0')
					return ((char *) &big[i]);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int main(void)
{
    const char *haystack = "Hello, this is a simple test string.";
    const char *needle = "simple";
    size_t len = 30;

    char *result = ft_strnstr(haystack, needle, len);

    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not found.\n");

    return 0;
}
//*/
