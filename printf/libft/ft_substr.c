/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:08:56 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/22 07:39:38 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*c;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		len = 0;
	len = ft_min(len, (ft_strlen(s) - start));
	c = malloc(len + 1);
	if (c != NULL)
	{
		if (len > 0)
			ft_memcpy(c, (s + start), len);
		c[len] = 0;
	}
	return (c);
}
