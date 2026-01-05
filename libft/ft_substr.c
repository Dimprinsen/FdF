/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:31 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 17:40:10 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	slen;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	if ((size_t)start > slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*
#include <stdio.h>
int	main(void)
{
	const char *source = "Hello, world!";
	char *sub;

	// Basic test
	sub = ft_substr(source, 7, 5);
	printf("ft_substr(\"%s\", 7, 5) = \"%s\"\n", source, sub);
	free(sub);

	// Start beyond length
	sub = ft_substr(source, 50, 5);
	printf("ft_substr(\"%s\", 50, 5) = \"%s\"\n", source, sub);
	free(sub);

	// Length exceeds bounds
	sub = ft_substr(source, 7, 50);
	printf("ft_substr(\"%s\", 7, 50) = \"%s\"\n", source, sub);
	free(sub);

	// Length 0
	sub = ft_substr(source, 7, 0);
	printf("ft_substr(\"%s\", 7, 0) = \"%s\"\n", source, sub);
	free(sub);

	// Full string
	sub = ft_substr(source, 0, strlen(source));
	printf("ft_substr(\"%s\", 0, %zu) = \"%s\"\n", source, strlen(source), sub);
	free(sub);

	// Empty input
	sub = ft_substr("", 0, 10);
	printf("ft_substr(\"\", 0, 10) = \"%s\"\n", sub);
	free(sub);

	// NULL input
	sub = ft_substr(NULL, 0, 10);
	printf("ft_substr(NULL, 0, 10) = %s\n", sub ? sub : "(null)");
	free(sub);

	return 0;
}//*/
