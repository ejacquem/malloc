/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:55:03 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/23 17:12:39 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*c;
	int		len;

	len = ft_strlen(s) + 1;
	c = malloc(len);
	if (c != NULL)
		ft_strlcpy(c, s, -1);
	return (c);
}
