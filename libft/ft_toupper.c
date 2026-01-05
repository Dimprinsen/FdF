/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:00:57 by thtinner          #+#    #+#             */
/*   Updated: 2025/05/13 16:49:40 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		return (c = c - 32);
	return (c);
}
/*#include <stdio.h>
int     main()
{
        char a = 'a';
        char b = 'z';
	char c = 'C';
        printf("%c\n", ft_toupper(a));
        printf("%c\n", ft_toupper(b));
	printf("%c\n", ft_toupper(c));
        return (0);
}
//*/
