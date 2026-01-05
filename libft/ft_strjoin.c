/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:39 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/21 20:57:25 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len + 1);
	ft_strlcat(str, s2, len + 1);
	return (str);
}
/*
#include <stdio.h>
int	main(void)
{
	const char *s1 = "Hello, ";
	const char *s2 = "World!";
	const char *expected = "Hello, World!";
	char *result = ft_strjoin(s1, s2);

	if (!result)
	{
		printf("❌ ft_strjoin returned NULL\n");
		return (1);
	}

	printf("Result:   \"%s\"\n", result);
	printf("Expected: \"%s\"\n", expected);

	if (strcmp(result, expected) == 0)
		printf("✅ Match!\n");
	else
		printf("❌ Mismatch\n");

	free(result);
	return (0);
}//*/