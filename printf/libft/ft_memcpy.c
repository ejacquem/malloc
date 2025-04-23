/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:47:01 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/20 08:18:47 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	int	i;

	if (src == dest || n == 0)
		return (dest);
	i = -1;
	while (++i < (int)n)
		*(BYTE *)(dest + i) = *(BYTE *)(src + i);
	return (dest);
}
