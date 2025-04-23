/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:33:40 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/27 08:09:24 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft/libft.h"
# include <stdarg.h>
# include <stdio.h>

# define FALSE 0
# define TRUE 1
# define MAX_INT 2147483647
# define MIN_INT -2147483648

typedef struct s_format
{
	int		n1;
	int		n2;
	int		right;
	char	space;
	int		capitals;
	char	prefix;
	char	ox[3];
	int		ispointer;
}				t_format;

int				ft_printf(const char *format, ...);

void			ft_printn(char c, int n);
void			ft_printp(unsigned long long n, t_format *t_f);
void			ft_printc(char c, t_format *t_f);
void			ft_prints(char *s, t_format *t_f);
void			ft_printnb(unsigned long long n, t_format *t_f, int base);

int				ft_logn(unsigned long long n, unsigned int base);
void			ft_putchar(char c);
void			ft_putnstr(char *s, unsigned int n);
void			ft_putu(unsigned long long n, unsigned int base, int capitals);

const char		*handle_args(const char *c, va_list args, t_format *t_f);
const char		*get_args(const char *c, t_format *t_f);
unsigned long	tounsigned(long n, t_format *t_f);
t_format		newf(void);

int				ft_max(int a, int b);
int				ft_min(int a, int b);
void			handle_zero_edge_case(t_format *t_f);

#endif
