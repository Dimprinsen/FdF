/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:00:43 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:33:56 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
/*
#include <stdio.h>
int	main()
{
	char a = 'b';
	char b = '9';
	char c = ' ';
	printf("%i\n", ft_isalnum(a));
	printf("%i\n", ft_isalnum(b));
	printf("%i\n", ft_isalnum(c));
	return (0);
}
//*/
