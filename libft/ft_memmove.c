/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:45:26 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 20:51:42 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*src1;
	size_t			i;
	unsigned char	*dest1;

	i = 0;
	src1 = (unsigned char *)src;
	dest1 = (unsigned char *)dest;
	if ((dest == NULL && src == NULL) || n == 0)
		return (dest);
	if (dest > src && (src1 + n) >= dest1)
	{
		while (n-- > 0)
		{
			dest1[n] = src1[n];
		}
	}
	else
	{
		while (i < n)
		{
			dest1[i] = src1[i];
			i++;
		}
	}
	return (dest1);
}
