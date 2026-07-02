/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 00:02:59 by adrouin           #+#    #+#             */
/*   Updated: 2024/10/17 15:29:04 by adrouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	result;
	int	a;
	int	i;

	result = 0;
	a = 0;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			a++;
		i++;
	}
	while (nptr[i] != '\0' && nptr[i] <= '9' && nptr[i] >= '0')
	{
		result = (result * 10) + (nptr[i] - 48);
		i++;
	}
	if (a % 2 != 0)
		result = result * (-1);
	return (result);
}
