/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:01:03 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:40:42 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		return (c = c + 32);
	return (c);
}
/*
#include <stdio.h>
int	main()
{
	char a = 'A';
	char b = 'Z';
	printf("%c\n", ft_tolower(a));
	printf("%c\n", ft_tolower(b));
	return (0);
}
//*/
