/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:47:53 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 19:15:25 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (dest[i] != '\0' && i < size)
		i++;
	j = i;
	while (src[i - j] != '\0' && i < size - 1)
	{
		dest[i] = *(char *)&src[i - j];
		i++;
	}
	if (j < size)
		dest[i] = '\0';
	return (j + ft_strlen(src));
}
