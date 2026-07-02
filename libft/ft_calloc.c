/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 00:03:21 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/21 23:34:01 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*t;
	size_t	i;
	size_t	max;

	max = (nmemb * size);
	if (!nmemb)
		return (malloc(0));
	t = malloc(max);
	if (t == NULL)
		return (0);
	i = 0;
	while (i < nmemb * size)
	{
		t[i] = 0;
		i++;
	}
	return (t);
}
