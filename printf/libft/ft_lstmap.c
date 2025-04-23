/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljacquem <ljacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 08:02:04 by ljacquem          #+#    #+#             */
/*   Updated: 2024/02/20 08:03:36 by ljacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*newlst;

	newlst = NULL;
	if (f == NULL || del == NULL)
		return (NULL);
	while (lst)
	{
		temp = ft_lstnew(f(lst->content));
		if (temp == NULL)
		{
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, temp);
		lst = lst->next;
	}
	return (newlst);
}

// /* This version does not del but still passes all online tests */
// t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
// {
// 	t_list *newlst;

// 	newlst = NULL;
// 	if(f == NULL || del == NULL)
// 		return NULL;
// 	while(lst)
// 	{
// 		ft_lstadd_back(&newlst, ft_lstnew(f(lst->content)));
// 		lst = lst->next;
// 	}
// 	return newlst;
// }