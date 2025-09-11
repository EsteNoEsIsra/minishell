/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:52:22 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/26 11:52:43 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_print_words(char **args, int i)
{
	int	w;

	while (args[i])
	{
		w = write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (w < 0)
			return (-1);
		if (args[i + 1])
		{
			w = write(STDOUT_FILENO, " ", 1);
			if (w < 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	ft_handle_echo(char **args)
{
	int	i;
	int	nflag;

	i = 1;
	nflag = 0;
	while (args[i] && !ft_strncmp(args[i], "-n", 3))
	{
		nflag = 1;
		i++;
	}
	if (ft_print_words(args, i) < 0)
		return (1);
	if (!nflag && write(STDOUT_FILENO, "\n", 1) < 0)
		return (1);
	return (0);
}

void	ft_echo(char **args, t_mini_sh *sh)
{
	sh->last_status = ft_handle_echo(args);
}
