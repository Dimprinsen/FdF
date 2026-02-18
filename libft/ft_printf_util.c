/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:01:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/06/18 17:16:55 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	putchar(int c)
{
	return (write (1, &c, 1));
}

int	putstr(char *str)
{
	int	count;

	count = 0;
	if (!str)
		return (count = write(1, "(null)", 6), count);
	while (*str != '\0')
	{
		putchar(*str++);
		count++;
	}
	return (count);
}

int	print_digit(long n)
{
	int		count;
	char	*symbols;

	symbols = "0123456789";
	if (n < 0)
	{
		write(1, "-", 1);
		return (print_digit(-n) + 1);
	}
	else if (n < 10)
		return (putchar(symbols[n]));
	else
	{
		count = print_digit(n / 10);
		return (count + print_digit(n % 10));
	}
}

int	print_hex(unsigned long n, char spec)
{
	int		count;
	char	*symbols;

	count = 0;
	if (spec == 'X')
		symbols = "0123456789ABCDEF";
	else if (spec == 'x' || spec == 'p')
		symbols = "0123456789abcdef";
	if (spec == 'p')
	{
		if (n == 0)
			return (write(1, "(nil)", 5));
		count += write(1, "0x", 2);
		spec = 'x';
	}
	if (n >= 16)
		count += print_hex(n / 16, spec);
	count += putchar(symbols[n % 16]);
	return (count);
}
