/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:03:51 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/19 11:06:43 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f) (unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
void	capitalize_even(unsigned int i, char *c)
{
	if (i % 2 == 0 && *c >= 'a' && *c <= 'z')
		*c = *c - 32;  // OR: *c = toupper(*c);
}
#include <stdio.h>
int	main(void)
{
	char str[] = "hello world";  // original string

	ft_striteri(str, capitalize_even);

	printf("Result: %s\n", str);  // Expected: "HeLlO WoRlD"

	return (0);
}
//*/
