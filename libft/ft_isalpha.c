/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:00:13 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:10:04 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}
/*
#include <stdio.h>
int	main()
{
	char a = 'a';
	char b = 'D';
	char c = ' ';
	printf("%i\n", ft_isalpha(a));
	printf("%i\n", ft_isalpha(b));
	printf("%i\n", ft_isalpha(c));
	return (0);
}//*/
