/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:26:32 by ljacquem          #+#    #+#             */
/*   Updated: 2025/05/02 14:44:17 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//print char c n times
void	ft_printn(char c, int n)
{
	while (n-- > 0)
		ft_putchar(c);
}

void	ft_printsn(char *s, int n)
{
	while (n-- > 0)
		ft_putnstr(s, ft_strlen(s));
}

void	ft_printnb(unsigned long long n, t_format *t_f, int base)
{
	const int	min_nb_len = ft_max(t_f->n2, ft_logn(n, base));

	if (t_f->ispointer && n != 0)
		t_f->n1 -= 2;
	if (n == 0 && t_f->n2 == 0)
		return (handle_zero_edge_case(t_f));
	if (t_f->prefix && t_f->space == '0')
		ft_putchar(t_f->prefix);
	if (t_f->ispointer && (!t_f->right || t_f->space == '0') && n != 0)
		ft_putnstr(t_f->ox, 2);
	if (t_f->right)
		ft_printn(t_f->space, t_f->n1 - min_nb_len - (t_f->prefix > 0));
	if (t_f->prefix && t_f->space == ' ')
		ft_putchar(t_f->prefix);
	if (t_f->ispointer && t_f->right && t_f->space != '0' && n != 0)
		ft_putnstr(t_f->ox, 2);
	ft_printn('0', min_nb_len - ft_logn(n, base));
	ft_putu(n, base, t_f->capitals);
	if (!t_f->right)
		ft_printn(t_f->space, t_f->n1 - min_nb_len - (t_f->prefix > 0));
}

void	ft_printc(char c, t_format *t_f)
{
	if (t_f->right)
		ft_printn(' ', t_f->n1 - 1);
	ft_putchar(c);
	if (!t_f->right)
		ft_printn(' ', t_f->n1 - 1);
}

void	ft_prints(char *s, t_format *t_f)
{
	int	spacenb;

	if (s == 0 && (t_f->n2 < 6 && t_f->n2 >= 0))
	{
		ft_printn(t_f->space, t_f->n1);
		return ;
	}
	if (s == NULL)
		return (ft_prints("(null)", t_f));
	if (t_f->n2 == -1)
		t_f->n2 = MAX_INT;
	spacenb = t_f->n1 - ft_min(ft_strlen(s), t_f->n2);
	if (t_f->right)
		ft_printn(' ', spacenb);
	ft_putnstr(s, ft_min(ft_strlen(s), t_f->n2));
	if (!t_f->right)
		ft_printn(' ', spacenb);
}

void	ft_printp(unsigned long long n, t_format *t_f)
{
	if (n == 0)
	{
		t_f->n2 = -1;
		return (ft_prints("(nil)", t_f));
	}
	t_f->ispointer = 1;
	ft_printnb(n, t_f, 16);
}

// void	ft_printnb(unsigned long long n, t_format *t_f, int base)
// {
// 	const int	min_nb_len = MAX(t_f->n2, ft_logn(n, base));

// 	if (n == 0 && t_f->n2 == 0)
// 	{
// 		if (t_f->n1)
// 			ft_printn(t_f->space, t_f->n1);
// 		return ;
// 	}

// 	char * prefix = get_prefix();
// 	int spaceBefore = 0;
// 	int spaceAfter = 0;
// 	int zeronb = min_nb_len - (int)ft_logn(n, base);

// 	if(t_f->right)
// 		spaceBefore = t_f->n1 - min_nb_len - strlen(t_f->prefix);
// 	if (!t_f->right)
// 		spaceAfter = t_f->n1 - min_nb_len - strlen(t_f->prefix);

// 	ft_printn(' ', spaceBefore);
// 	ft_putnstr(prefix);
// 	ft_printn('0', zeronb);
// 	ft_putu(n, base, t_f->capitals);
// 	ft_printn(' ', spaceAfter);
// }
