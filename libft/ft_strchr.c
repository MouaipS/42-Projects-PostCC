/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:54:02 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/22 04:57:53 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*s2;
	char	d;

	i = 0;
	d = (char)c;
	s2 = (char *)s;
	while (s2[i] != '\0')
	{
		if (s2[i] == d)
			return (s2 + i);
		i++;
	}
	if (s2[i] == d)
		return (s2 + i);
	return (NULL);
}
