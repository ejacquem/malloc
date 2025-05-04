/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:34:34 by ljacquem          #+#    #+#             */
/*   Updated: 2025/05/04 12:28:59 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long	ft_min(long a, long b)
{
	if (a < b)
		return (a);
	return (b);
}

long	ft_max(long a, long b)
{
	if (a > (b))
		return (a);
	return (b);
}

void	handle_zero_edge_case(t_format *t_f)
{
	if (t_f->prefix == '+')
	{
		if (t_f->right)
			ft_printn(t_f->space, t_f->n1 - 1);
		ft_putchar('+');
		if (!t_f->right)
			ft_printn(t_f->space, t_f->n1 - 1);
		return ;
	}
	return (ft_printn(' ', t_f->n1));
}
