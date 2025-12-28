/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thtinner <thtinner@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:35:20 by thtinner          #+#    #+#             */
/*   Updated: 2025/08/25 20:55:41 by thtinner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <limits.h>

int	putchar(int c);
int	putstr(char *str);
int	print_digit(long n);
int	print_hex(long n, char spec);
int	print_pointer(unsigned long n);
int	ft_printf(const char *str, ...);

#endif
