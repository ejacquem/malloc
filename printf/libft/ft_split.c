/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:52:18 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/20 09:29:02 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char sep)
{
	int	i;

	i = 0;
	if (sep == 0)
		return (1);
	while (*s == sep)
		s++;
	while (*s)
	{
		i++;
		while (*s != sep && *s)
			s++;
		while (*s == sep && *s)
			s++;
	}
	return (i);
}

static int	next_sep(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		len;
	int		i;

	if (s == NULL)
		return (NULL);
	tab = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s == 0)
			break ;
		len = next_sep(s, c);
		tab[i] = ft_substr(s, 0, len);
		s += len;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
