/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:51:23 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/22 07:32:15 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_abs(long i)
{
	if (i < 0)
		i *= -1;
	return (i);
}

static void	recurse(long n, int fd)
{
	if (n > 9)
		recurse(n / 10, fd);
	ft_putchar_fd(n % 10 + '0', fd);
}

void	ft_putnbr_fd(long n, int fd)
{
	if (n < 0)
		ft_putchar_fd('-', fd);
	recurse(ft_abs(n), fd);
}
