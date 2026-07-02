/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 04:47:35 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/17 19:59:32 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char		*ft_strcpy(char *dest, const char *src);
static char		*ft_strcpy2(char *dest, const char *src, const char *s1);

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	longs;
	char	*tab;

	if (!s1 || !s2)
		return (NULL);
	longs = ft_strlen(s1) + ft_strlen(s2);
	tab = malloc(sizeof(char) * (longs + 1));
	if (!tab)
		return (NULL);
	tab = ft_strcpy(tab, s1);
	tab = ft_strcpy2(tab, s2, s1);
	return (tab);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

static char	*ft_strcpy2(char *dest, const char *src, const char *s1)
{
	int	i;
	int	a;

	i = 0;
	a = ft_strlen(s1);
	while (src[i] != '\0')
	{
		dest[i + a] = src[i];
		i++;
	}
	dest[i + a] = '\0';
	return (dest);
}
