/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:48:12 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/20 16:37:57 by israetor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void ft_getinput(t_mini_sh*sh)
{
	while (1)
	{
		set_signals_interactive();
		sh->input = readline(GRN "Minishell$> " NRM);
	//	set_signals_noninteractive();
		if(g_signal_vol)
			g_signal_vol = 0; // Reset signal after handling
		else if (g_signal_vol == SIGQUIT)
			g_signal_vol = 0;
		if (!sh->input) // Ctrl+D  hace que readline retorne NULL
		{
			printf("exit\n");
			ft_free_mini_sh(sh, 1);
			rl_clear_history();
			exit(EXIT_SUCCESS);
		}
		if (sh->input[0] == ' ' || sh->input[0] == '\0')
		{
			free(sh->input);
			ft_printf("");
//			rl_on_new_line(); //mueve el cursor a una nueva linea
 //   			rl_replace_line("", 0);  //borra el texto de la linea actual
 //   			rl_redisplay(); //redibuja la linea actual
			continue ;
		}
		add_history(sh->input);
		
		//ft_printf("Has escrito " BLU "%s\n" NRM,input);//para debug
		sh->tokens = tokenizer(sh->input, sh);
		if (!sh->tokens)
		{
			free(sh->input);
			continue ;
		}
		sh->tokens_head = sh->tokens;
		/*while (sh->tokens)
		{
			dprintf(2, "TOKEN %d: %s\n", sh->tokens->type, sh->tokens->value);//para debug
			sh->tokens = sh->tokens->next;
		}
		sh->tokens = sh->tokens_head;*/

		sh->node = parse(&sh->tokens);
		if (!sh->node)
		{
			ft_free_mini_sh(sh, 0);
			continue ;
		}
		sh->node_head = sh->node;
		//print_ast(sh->node, 0);
		expand_heredocs(sh->node);
		if (sh->node->type == NODE_PIPE)
		{	//node = node->left;
			ft_execute_pipe(sh->node, sh);
		}
		else
			ft_execute(sh->node, sh);
		//printf("*-*" BLU "%s" NRM "*-*\n",node->args[0]);
		//if(node->args[0][0] == '/')   para otras opciones
		ft_free_mini_sh(sh, 0);
	}

}

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
