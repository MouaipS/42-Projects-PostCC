/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:59:35 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 19:20:11 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	int				a;
	unsigned char	*t1;
	unsigned char	*t2;

	i = 0;
	a = 0;
	t2 = (unsigned char *)s2;
	t1 = (unsigned char *)s1;
	while ((i != n))
	{
		if (t1[i] != t2[i])
		{
			a = t1[i] - t2[i];
			return (a);
		}
		i++;
	}
	return (0);
}
