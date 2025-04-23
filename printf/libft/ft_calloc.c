/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:18:45 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/19 16:09:50 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//return malloc(1); test OK but might be wrong, val not set to 0
void	*ft_calloc(size_t n, size_t size)
{
	void	*ptr;

	if (n == 0 || size == 0)
		return (malloc(1));
	if (n > INT_MAX || size > INT_MAX || n * size > INT_MAX)
		return (NULL);
	ptr = (void *)malloc(n * size);
	if (ptr)
		ft_bzero(ptr, n * size);
	return (ptr);
}
