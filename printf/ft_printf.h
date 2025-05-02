/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:33:40 by ljacquem          #+#    #+#             */
/*   Updated: 2025/05/02 14:44:20 by lucas            ###   ########.fr       */
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


#define COLOR

#ifdef COLOR
	#define COLOR_PRINT_RESET	write(1, "\x1b[0m", 4)
	#define COLOR_PRINT_YELLOW	write(1, "\x1b[38;5;228m", 11)
	#define COLOR_PRINT_SKY		write(1, "\x1b[38;5;123m", 11)
#else
	#define COLOR_PRINT_RESET	
	#define COLOR_PRINT_YELLOW	
	#define COLOR_PRINT_SKY		
#endif

// #define COLOR_PRINT_BLACK	write(1, "\x1b[30m", 5)
// #define COLOR_PRINT_RED		write(1, "\x1b[31m", 5)
// #define COLOR_PRINT_GREEN	write(1, "\x1b[32m", 5)
// #define COLOR_PRINT_BLUE	write(1, "\x1b[34m", 5)
// #define COLOR_PRINT_MAGENTA	write(1, "\x1b[35m", 5)
// #define COLOR_PRINT_CYAN	write(1, "\x1b[36m", 5)
// #define COLOR_PRINT_WHITE	write(1, "\x1b[37m", 5)

#define COLOR_PRINT_NUMBER COLOR_PRINT_YELLOW
#define COLOR_PRINT_POINTER COLOR_PRINT_SKY

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
void			ft_printsn(char *s, int n);
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
