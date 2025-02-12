/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:30:21 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/07/21 22:30:23 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(char *str)
{
	int		i;
	int		sign;
	long	num;

	num = 0;
	i = 0;
	sign = atoi_get_sign(&str);
	if (!str[i])
		return (LONG_MAX - 3);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		if (num > INT_MAX)
			return (LONG_MAX - 3);
		num += str[i++] - 48;
		if (str[i] == '\0')
			return (num * sign);
	}
	if (str[i])
		return (LONG_MAX - 3);
	return (-1);
}

int	atoi_get_sign(char **str)
{
	int		sign;
	char	*stri;
	int		i;

	i = 0;
	stri = *str;
	sign = 1;
	while (stri[i] == 32 || (stri[i] >= 9 && stri[i] <= 13))
		i++;
	if (stri[i] == '+' || stri[i] == '-')
	{
		if (stri[i] == '-')
			sign *= -1;
		i++;
	}
	*str = *str + i;
	return (sign);
}
