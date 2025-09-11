/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:54:23 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/26 11:54:24 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pwd(t_mini_sh *sh)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		sh->last_status = 0;
	}
	else
	{
		ft_put_error("pwd", "error retrieving current directory");
		sh->last_status = 1;
	}
}
