/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:40:19 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/16 19:20:22 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_numlen(int a);

char	*ft_itoa(int n)
{
	int		numlen;
	char	*tab;
	int		i;

	i = 0;
	numlen = ft_numlen(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	tab = malloc(sizeof(char) * (numlen + 1));
	if (tab == 0)
		return (NULL);
	if (n < 0)
	{
		tab[0] = '-';
		n = n * -1;
		i++;
	}
	tab[numlen] = '\0';
	while (numlen > i)
	{
		tab[numlen - 1] = (n % 10) + '0';
		n /= 10;
		numlen--;
	}
	return (tab);
}

static int	ft_numlen(int n)
{
	int	numlen;

	numlen = 0;
	if (n == 0)
	{
		numlen++;
		return (numlen);
	}
	if (n < 0)
	{
		n = n * -1;
		numlen++;
	}
	while (n != 0)
	{
		n = n / 10;
		numlen++;
	}
	return (numlen);
}
