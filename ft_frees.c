/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_frees.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:46:16 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 18:52:02 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	ft_free_ast(t_ast *head)
{
	if (!head)
		return ;
	ft_free_ast(head->left);
	ft_free_ast(head->right);
	if (head->args)
		ft_free_strs(head->args);
	if (head->filename)
		free(head->filename);
	free(head);
}

void	ft_free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	ft_free_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->name);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	ft_free_mini_sh(t_mini_sh *sh, int env)
{
	if (env)
		ft_free_env(sh->env);
	if (sh->tokens_head)
	{
		ft_free_tokens(sh->tokens_head);
		sh->tokens = NULL;
		sh->tokens_head = NULL;
	}
	if (sh->node_head)
	{
		ft_free_ast(sh->node_head);
		sh->node = NULL;
		sh->node_head = NULL;
	}
	if (sh->input)
		free(sh->input);
}
