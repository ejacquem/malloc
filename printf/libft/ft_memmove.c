/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:47:24 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/20 08:18:47 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (src >= dest)
		return (ft_memcpy(dest, src, n));
	while (n--)
		*(BYTE *)(dest + n) = *(BYTE *)(src + n);
	return (dest);
}
