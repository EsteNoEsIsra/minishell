/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:48:12 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 18:22:00 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int argc, char**argv, char **envp)
{
	t_mini_sh	shell;

	(void)argv;
	if (argc == 1)
	{
		shell.mypid = getpid();
		shell.input = 0;
		shell.myfd = 1;	
		shell.env = ft_setenv(envp);
		shell.last_status = 0;
		shell.tokens = NULL;
		shell.tokens_head = NULL;
		shell.node = NULL;
		shell.node_head = NULL;
	 	
	//	ft_setup_signals(shell);
	//	set_signals_interactive();
		ft_getinput(&shell);

	}
	else  // solo para saltar la funcion no usada  preguntar donde esta el heredoc para implementarlo
		set_signals_noninteractive();

	return (1);
}
