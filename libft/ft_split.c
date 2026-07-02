/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:37:21 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/21 23:28:16 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_separator(const char *s, char c);
static char	**ft_tabfree(char **tab, int i);

char	**ft_split(char const *s, char c)
{
	int		i;
	size_t	len;
	char	**tab;

	i = 0;
	tab = (char **)ft_calloc(sizeof(char *), (ft_separator(s, c) + 1));
	if (!tab)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			if (!ft_strchr(s, c))
				len = ft_strlen(s);
			else
				len = ft_strchr(s, c) - s;
			tab[i++] = ft_substr(s, 0, len);
			if (!tab[i - 1])
				return (ft_tabfree(tab, i));
			s += len;
		}
	}
	return (tab);
}

static int	ft_separator(const char *s, char c)
{
	size_t	a;

	a = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			a++;
		while (*s && *s != c)
			s++;
	}
	return (a);
}

static char	**ft_tabfree(char **tab, int i)
{
	while (i != 0)
	{
		i--;
		free(tab[i]);
	}
	free(tab);
	return (NULL);
}
