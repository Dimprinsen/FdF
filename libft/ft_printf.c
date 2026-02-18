/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:00:41 by thtinner          #+#    #+#             */
/*   Updated: 2025/06/18 17:43:24 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_format(char spec, va_list ap)
{
	int	count;

	count = 0;
	if (spec == 'c')
		count += putchar(va_arg(ap, int));
	else if (spec == 's')
		count += putstr(va_arg(ap, char *));
	else if (spec == 'p')
		count += print_hex(va_arg(ap, unsigned long), 'p');
	else if (spec == 'd' || spec == 'i')
		count += print_digit(va_arg(ap, int));
	else if (spec == 'u')
		count += print_digit(va_arg(ap, unsigned int));
	else if (spec == 'x')
		count += print_hex(va_arg(ap, unsigned int), 'x');
	else if (spec == 'X')
		count += print_hex(va_arg(ap, unsigned int), 'X');
	else if (spec == '%')
		count += putchar('%');
	else
		count += write(1, &spec, 1);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		count;
	va_list	ap;

	count = 0;
	va_start(ap, str);
	while (*str)
	{
		if (*str == '%')
			count += check_format(*(++str), ap);
		else
			count += write(1, str, 1);
		str++;
	}
	va_end(ap);
	return (count);
}
/*#include <stdio.h>
#include <limits.h>
#include "ft_printf.h"

int main(void)
{
	int		ret1, ret2;
	char	*str = "hello";
	char	c = 'A';
	int		num = -12345;
	unsigned int u = 4294967295U;
	void	*ptr = str;

	ret1 = ft_printf("ft_printf: [%c]\n", c);
	ret2 = printf(    "printf:    [%c]\n", c);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%s]\n", str);
	ret2 = printf(    "printf:    [%s]\n", str);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%p]\n", ptr);
	ret2 = printf(    "printf:    [%p]\n", ptr);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%d] [%i]\n", num, num);
	ret2 = printf(    "printf:    [%d] [%i]\n", num, num);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%u]\n", u);
	ret2 = printf(    "printf:    [%u]\n", u);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%x] [%X]\n", u, u);
	ret2 = printf(    "printf:    [%x] [%X]\n", u, u);
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ret1 = ft_printf("ft_printf: [%%]\n");
	ret2 = printf(    "printf:    [%%]\n");
	printf("ft_printf returned: %d\n", ret1);
	printf("printf returned:    %d\n\n", ret2);
	ft_printf("ft_printf returned: %d\n", ret1);
	printf(    "printf returned:    %d\n", ret2);
	return (0);
}*/