/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:13:44 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 18:37:03 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (s1 && set)
	{
		i = 0;
		j = ft_strlen(s1);
		while (s1[i] && ft_strchr(set, s1[i]))
			i++;
		while (s1[j - 1] && ft_strchr(set, s1[j - 1]) && j > i)
			j--;
		str = malloc(sizeof(char) * (j - i + 1));
		if (!str)
			return (NULL);
		if (str)
		{
			ft_strlcpy(str, &s1[i], j - i + 1);
			return (str);
		}
	}
	return (NULL);
}
/*#include <stdio.h>
int	main(void)
{
	const char *s1 = "   \t\nHello, world!  \n\t  ";
	const char *set = " \n\t";
	char *trimmed;

	trimmed = ft_strtrim(s1, set);
	if (trimmed)
	{
		printf("Original: \"%s\"\n", s1);
		printf("Trimmed:  \"%s\"\n", trimmed);
		free(trimmed);
	}
	else
		printf("ft_strtrim returned NULL\n");

	// Additional test cases
	char *test_cases[][2] = {
		{"---42---", "-"},
		{"nothing to trim", ""},
		{"xxxxx", "x"},
		{"   abc   ", " "},
		{"", " "},
		{"   ", " "},
		{NULL, NULL} // Sentinel to end loop
	};

	printf("\nMore test cases:\n");
	for (int i = 0; test_cases[i][0]; i++)
	{
		char *res = ft_strtrim(test_cases[i][0], test_cases[i][1]);
		printf("Input: \"%s\", Set: \"%s\" => Result: \"%s\"\n",
			test_cases[i][0], test_cases[i][1], res ? res : "(null)");
		free(res);
	}

	return 0;
}//*/
