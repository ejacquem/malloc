/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:34:34 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/27 08:09:31 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	ft_max(int a, int b)
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
