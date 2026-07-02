/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:46:36 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 19:15:10 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i])
			i++;
		return (i);
	}
	while ((src[i] != '\0') && (i < size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	if (i < size)
		dest[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}
