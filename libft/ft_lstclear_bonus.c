/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 20:49:57 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/18 21:47:42 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*a;

	if (!del || !lst || !*lst)
		return ;
	while (lst && *lst)
	{
		a = (*lst)->next;
		ft_lstdelone(*lst, *del);
		*lst = a;
	}
}
