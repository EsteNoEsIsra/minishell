/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:22:33 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:28:12 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = value;
	new->next = NULL;
	return (new);
}

int	validate_command(t_ast *node)
{
	t_ast	*bottom;

	bottom = node;
	while (bottom && bottom->type == NODE_REDIR)
		bottom = bottom->left;
	if (!bottom || !bottom->args || !bottom->args[0])
	{
		ft_put_error("minishell", "syntax error: empty command");
		ft_free_ast(node);
		return (0);
	}
	return (1);
}

int	is_redir(t_token_type type)
{
	int	result;

	result = 0;
	if (type == TOKEN_REDIR_OUT)
		result = 1;
	else if (type == TOKEN_HEREDOC)
		result = 1;
	else if (type == TOKEN_REDIR_IN)
		result = 1;
	else if (type == TOKEN_REDIR_APPEND)
		result = 1;
	return (result);
}

void	child_heredoc(int *fd, char *delimiter)
{
	char	*line;

	close(fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}
