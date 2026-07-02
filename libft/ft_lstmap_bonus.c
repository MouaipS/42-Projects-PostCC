/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 23:34:23 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/22 02:25:01 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*first;

	if (!f || !lst || !del)
		return (lst);
	new_list = ft_lstnew(f(lst->content));
	if (!new_list)
		return (NULL);
	first = new_list;
	lst = lst->next;
	while (lst)
	{
		new_list->next = ft_lstnew(f(lst->content));
		if (!new_list->next)
		{
			ft_lstclear(&first, del);
			return (0);
		}
		lst = lst->next;
		new_list = new_list->next;
	}
	new_list->next = NULL;
	return (first);
}
