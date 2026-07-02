/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:43:41 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 20:12:24 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*src1;
	unsigned char	*dest1;
	size_t			i;

	i = 0;
	src1 = (unsigned char *)src;
	dest1 = (unsigned char *)dest;
	if (dest == NULL && src == NULL)
		return (NULL);
	if (n == 0)
		return (dest);
	while (i != n)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest);
}
