/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getinput.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:18:01 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:05:33 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	handle_eof(t_mini_sh *sh)
{
	if (sh->input != NULL)
		return ;
	printf("exit\n");
	ft_free_mini_sh(sh, 1);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

static int	skip_blank(t_mini_sh *sh)
{
	if (sh->input[0] == '\0' || sh->input[0] == ' ')
	{
		free(sh->input);
		ft_printf("");
		return (1);
	}
	return (0);
}

static int	tokenize_and_parse(t_mini_sh *sh)
{
	add_history(sh->input);
	sh->tokens = tokenizer(sh->input, sh);
	if (sh->tokens == NULL)
	{
		free(sh->input);
		return (1);
	}
	sh->tokens_head = sh->tokens;
	sh->node = parse(&sh->tokens);
	if (sh->node == NULL)
	{
		ft_free_mini_sh(sh, 0);
		return (1);
	}
	sh->node_head = sh->node;
	return (0);
}

static void	exec_and_cleanup(t_mini_sh *sh)
{
	expand_heredocs(sh->node);
	if (sh->node->type == NODE_PIPE)
		ft_execute_pipe(sh->node, sh);
	else
		ft_execute(sh->node, sh);
	ft_free_mini_sh(sh, 0);
}

void	ft_getinput(t_mini_sh *sh)
{
	while (1)
	{
		set_signals_interactive();
		sh->input = readline(GRN "Minishell$> " NRM);
		if (g_signal_vol != 0)
			g_signal_vol = 0;
		handle_eof(sh);
		if (skip_blank(sh))
			continue ;
		if (tokenize_and_parse(sh))
			continue ;
		exec_and_cleanup(sh);
	}
}
