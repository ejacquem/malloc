/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:39:43 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/19 16:59:11 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//check null only to pass libft-unit-test
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	total;
	size_t	len;

	if ((src == 0 || dst == 0) && size == 0)
		return (0);
	len = ft_strlen(dst);
	total = len + ft_strlen(src);
	if (len >= size)
		return (ft_strlen(src) + size);
	size -= len;
	while (*src != 0 && --size > 0)
		dst[len++] = *src++;
	dst[len] = 0;
	return (total);
}
