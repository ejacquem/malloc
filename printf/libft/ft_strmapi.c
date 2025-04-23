/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:00:36 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/19 17:01:07 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*c;
	int		i;
	int		len;

	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	c = malloc(len + 1);
	if (c == NULL)
		return (NULL);
	i = -1;
	while (s[++i])
		c[i] = f(i, s[i]);
	c[len] = 0;
	return (c);
}
