/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:07:42 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:19:01 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_ast	*new_node(t_node_type type, char **args, char *str, t_token_type r_type)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->type = type;
	new->args = args;
	new->filename = str;
	new->redir_type = r_type;
	new->heredoc_fd = -1;
	new->right = NULL;
	new->left = NULL;
	return (new);
}

t_ast	*parse_redir(t_token **tokens, t_ast *left)
{
	t_ast			*redir;
	char			*filename;
	t_token_type	redir_type;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_put_error("minishell", "syntax error near redirection");
		ft_free_ast(left);
		return (NULL);
	}
	filename = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
	redir = new_node(NODE_REDIR, NULL, filename, redir_type);
	if (!redir)
		return (ft_free_ast(left), NULL);
	redir->left = left;
	return (redir);
}

int	cmd_args_append(t_ast *cmd, char *value)
{
	int		n;
	int		i;
	char	**args;

	n = 0;
	i = 0;
	while (cmd->args && cmd->args[n])
		n++;
	args = malloc(sizeof(char *) * (n + 2));
	if (!args)
		return (0);
	while (i < n)
	{
		args[i] = cmd->args[i];
		i++;
	}
	args[n] = ft_strdup(value);
	if (!args[n])
		return (free(args), 0);
	args[n + 1] = NULL;
	free(cmd->args);
	cmd->args = args;
	return (1);
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;
	t_ast	*left;

	cmd = new_node(NODE_COMMAND, NULL, NULL, TOKEN_NONE);
	if (!cmd)
		return (NULL);
	left = cmd;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!cmd_args_append(cmd, (*tokens)->value))
				return (ft_free_ast(left), NULL);
			*tokens = (*tokens)->next;
		}
		else if (is_redir((*tokens)->type))
		{
			left = parse_redir(tokens, left);
			if (!left)
				return (NULL);
		}
	}
	if (!validate_command(left))
		return (NULL);
	return (left);
}

t_ast	*parse(t_token **tokens)
{
	t_ast	*left;
	t_ast	*pipe;

	left = parse_command(tokens);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		pipe = new_node(NODE_PIPE, NULL, NULL, TOKEN_NONE);
		if (!pipe)
			return (ft_free_ast(left), NULL);
		pipe->left = left;
		pipe->right = parse(tokens);
		if (!pipe->right)
		{
			ft_free_ast(pipe);
			return (NULL);
		}
		return (pipe);
	}
	return (left);
}
