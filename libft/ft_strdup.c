/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 00:04:47 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/17 15:33:02 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*t;
	int				i;
	char			*s1;

	s1 = (char *)s;
	i = 0;
	t = (char *)malloc((ft_strlen(s)+1) * sizeof(char));
	if (t == NULL)
		return (0);
	while (s1[i] != '\0')
	{
		t[i] = s1[i];
		i++;
	}
	t[i] = '\0';
	return (t);
}
