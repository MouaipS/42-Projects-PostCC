/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:57:11 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 21:09:22 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	int				a;

	i = 0;
	a = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && (i < n))
	{
		if (s1[i] != s2[i])
		{
			a = *(unsigned char *)&s1[i] - *(unsigned char *)&s2[i];
			return (a);
		}
		i++;
	}
	a = 0;
	return (a);
}
