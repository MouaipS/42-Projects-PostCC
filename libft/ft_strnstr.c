/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 00:00:38 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/17 18:04:51 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	a;
	char	*big2;
	char	*little2;

	big2 = (char *)big;
	little2 = (char *)little;
	i = 0;
	while (big2[i] && i < len)
	{
		a = 0;
		while (little2[a] != '\0' && big2[i + a] == little2[a] && (i + a) < len)
		{
			a++;
		}
		if (little2[a] == '\0')
		{
			return (big2 + i);
		}
		i++;
	}
	if (little2[0] == '\0')
		return (big2);
	return (NULL);
}
