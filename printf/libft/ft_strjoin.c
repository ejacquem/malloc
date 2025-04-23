/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:57:06 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/22 07:39:06 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	char	*c;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	c = malloc(len + 1);
	if (c)
	{
		ft_strlcpy(c, s1, len + 1);
		ft_strlcat(c, s2, len + 1);
		c[len] = 0;
	}
	return (c);
}
