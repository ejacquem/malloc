/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:33:37 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/27 08:10:12 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

const char	g_hexdigits[] = "0123456789abcdef0123456789ABCDEF";

t_format	newf(void)
{
	t_format	t;

	t.n1 = 0;
	t.n2 = -1;
	t.right = TRUE;
	t.space = ' ';
	t.capitals = 0;
	t.prefix = 0;
	t.ispointer = 0;
	ft_memcpy(t.ox, "0x", 3);
	return (t);
}

unsigned long	tounsigned(long n, t_format *t_f)
{
	if (n < 0)
	{
		n *= -1;
		t_f->prefix = '-';
	}
	t_f->ispointer = FALSE;
	return (n);
}

void	ft_putu(unsigned long long n, unsigned int base, int capitals)
{
	if (n >= base)
		ft_putu(n / base, base, capitals);
	ft_putchar(g_hexdigits[(n % base) + capitals]);
}

void	ft_putnstr(char *s, unsigned int n)
{
	if (s == NULL)
		return (ft_putnstr("(null)", -1));
	while (*s && n--)
		ft_putchar(*s++);
}

int	ft_logn(unsigned long long n, unsigned int base)
{
	if (n >= base)
		return (ft_logn(n / base, base) + 1);
	return (1);
}
