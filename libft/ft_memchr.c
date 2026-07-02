/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:58:31 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 21:16:04 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (NULL);
	while (i < n)
	{
		if (*(unsigned char *)&s[i] == *(unsigned char *)&c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
