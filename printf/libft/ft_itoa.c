/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:32:25 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/22 07:33:03 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_abs(long i)
{
	if (i < 0)
		i *= -1;
	return (i);
}

static int	ft_log10(int n)
{
	if (n)
		return (ft_log10(n / 10) + 1);
	return (0);
}

static void	recurse(int i, char *a)
{
	if (i > 9)
		recurse(i / 10, a - 1);
	*a = i % 10 + '0';
}

char	*ft_itoa(int i)
{
	char	*a;
	int		len;

	if (i == 0)
		return (ft_strdup("0"));
	if (i == -2147483648)
		return (ft_strdup("-2147483648"));
	len = ft_log10(ft_abs(i)) + (i < 0);
	a = malloc(len + 1);
	if (a == NULL)
		return (NULL);
	*a = '-';
	recurse(ft_abs(i), a + len - 1);
	a[len] = 0;
	return (a);
}
