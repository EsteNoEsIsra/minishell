/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builting_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: israetor <israetor@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:48:00 by israetor          #+#    #+#             */
/*   Updated: 2025/09/11 17:48:02 by israetor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_overflow(long long num, int digit, int sign)
{
	if (sign == 1)
	{
		if (num > (LLONG_MAX - digit) / 10)
			return (0);
	}
	else
	{
		if (-num < (LLONG_MIN + digit) / 10)
			return (0);
	}
	return (1);
}

int	checklonglong(char *str)
{
	int			i;
	long long	num;
	int			sign;
	int			digit;

	i = 0;
	num = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (!check_overflow(num, digit, sign))
			return (0);
		num = num * 10 + digit;
		i++;
	}
	return (1);
}
