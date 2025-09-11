/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bexit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:50:51 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 18:49:20 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strslen(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		i++;
	return (i);
}

int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_parse_exit_code(char *str)
{
	int	nbr;
	int	neg;

	nbr = 0;
	neg = 0;
	
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			neg = 1;
		str++;
	}
	while (*str)
	{
		nbr = nbr * 10 + (*str - '0');
		str++;
	}
	if (neg)
		nbr = -nbr;
	return ((unsigned char)nbr);
}

int	checklonglong(char *str)
{
	int			i;
	long long	num;
	int			sign;

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
		num = num * 10 + (str[i] - '0');
		if ((num * sign) > 2147483647 || (num * sign) < -2147483648)
			return (0);
		i++;
	}
	return (1);
}

void	ft_bexit(char **args, t_mini_sh *sh)
{
	int	argc;
	int	exit_status;

	argc = ft_strslen(args);
	ft_putstr_fd("exit\n", 1);
	if (argc >= 2)
	{
		if (ft_is_valid_number(args[1]) && argc == 2)
			exit_status = ft_parse_exit_code(args[1]);
		else if (!ft_is_valid_number(args[1]) && argc >= 2)
		{
			ft_put_error("exit", "numeric argument required");
			exit_status = 2;
		}
		else
		{
			sh->last_status = 1;
			return (ft_put_error("exit", "too many arguments"));
		}
	}
	if (argc == 1)
		exit_status = sh->last_status;
	ft_free_mini_sh(sh, 1);
	rl_clear_history();
	exit(exit_status);
}
