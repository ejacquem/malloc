/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:33:44 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/27 17:36:18 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			g_length;

int	ft_printf(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	g_length = 0;
	while (*format)
	{
		if (*format == '%')
		{
			t_format f = newf();
			format = handle_args(++format, args, &f);
		}
		else
			ft_putchar(*format++);
	}
	va_end(args);
	return (g_length);
}

//returns ptr to skip args
const char	*handle_args(const char *c, va_list args, t_format *t_f)
{
	c = get_args(c, t_f);
	if (*c == '%')
		ft_putchar('%');
	else if (*c == 'c')
		ft_printc(va_arg(args, int), t_f);
	else if (*c == 's')
		ft_prints(va_arg(args, char *), t_f);
	else if (*c == 'p')
		ft_printp(va_arg(args, unsigned long long), t_f);
	else if (*c == 'd' || *c == 'i')
		ft_printnb(tounsigned(va_arg(args, int), t_f), t_f, 10);
	else if (*c == 'u')
		ft_printnb(tounsigned(va_arg(args, unsigned int), t_f), t_f, 10);
	else if (*c == 'x')
		ft_printnb(va_arg(args, unsigned int), t_f, 16);
	else if (*c == 'X')
	{
		t_f->ox[1] = 'X';
		t_f->capitals = 16;
		ft_printnb(va_arg(args, unsigned int), t_f, 16);
	}
	else if (*c == 'l' && *(c + 1) == 'd')
	{
		ft_printnb(tounsigned(va_arg(args, long), t_f), t_f, 10);
		return (c + 2);
	}
	return (c + 1);
}

void	check_flags(char c, t_format *t_f)
{
	if (c == '+')
		t_f->prefix = '+';
	if (c == ' ')
		t_f->prefix = ' ';
	if (c == '#')
		t_f->ispointer = TRUE;
	if (c == '0')
		t_f->space = '0';
	if (c == '-')
		t_f->right = FALSE;
}

const char	*get_args(const char *c, t_format *t_f)
{
	while (ft_strchr(" +-0#", *c))
	{
		check_flags(*c, t_f);
		c++;
	}
	if (ft_isdigit(*c))
	{
		t_f->n1 = ft_atoi(c);
		c += ft_logn(t_f->n1, 10);
	}
	if (*c == '.')
	{
		c++;
		t_f->n2 = 0;
		t_f->space = ' ';
		if (ft_isdigit(*c))
		{
			t_f->n2 = ft_atoi(c);
			c += ft_logn(t_f->n2, 10);
		}
	}
	return (c);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
	g_length++;
}
