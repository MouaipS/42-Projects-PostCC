/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:56:12 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/22 04:58:34 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	d;
	char	*s2;

	d = (char)c;
	i = 0;
	s2 = (char *)s;
	while (s2[i] != '\0')
		i++;
	while (i >= 0)
	{
		if (s2[i] == d)
			return (s2 + i);
		i--;
	}
	return (NULL);
}
