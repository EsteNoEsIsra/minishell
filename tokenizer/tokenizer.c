/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:08:19 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:23:38 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	handle_pipe(t_tokctx *c)
{
	t_token	*tok;

	if (!c->last)
		return ;
	tok = new_token(TOKEN_PIPE, ft_strdup("|"));
	c->last->next = tok;
	c->last = tok;
	c->i++;
}

void	handle_redirection(char *line, t_tokctx *c)
{
	t_token	*tok;

	if (line[c->i] == '<' && line[c->i + 1] == '<')
	{
		tok = new_token(TOKEN_HEREDOC, ft_strdup("<<"));
		c->i++;
	}
	else if (line[c->i] == '<')
		tok = new_token(TOKEN_REDIR_IN, ft_strdup("<"));
	else if (line[c->i] == '>' && line[c->i + 1] == '>')
	{
		tok = new_token(TOKEN_REDIR_APPEND, ft_strdup(">>"));
		c->i++;
	}
	else
		tok = new_token(TOKEN_REDIR_OUT, ft_strdup(">"));
	if (!c->head)
		c->head = tok;
	else
		c->last->next = tok;
	c->last = tok;
	c->i++;
}

static void	handle_word(char *line, t_tokctx *c, t_mini_sh *sh)
{
	char	*word;
	t_token	*tok;

	word = ft_build_word(line, &c->i, sh);
	if (!word || !*word)
		return (free(word));
	tok = new_token(TOKEN_WORD, word);
	if (!c->head)
		c->head = tok;
	else if (c->last)
		c->last->next = tok;
	c->last = tok;
}

static void	ft_build_tokens(char *line, t_tokctx *c, t_mini_sh *sh)
{
	while (line[c->i])
	{
		if (line[c->i] == '|')
			handle_pipe(c);
		else if (line[c->i] == '>' || line[c->i] == '<')
			handle_redirection(line, c);
		else if (line[c->i] == ' ')
			c->i++;
		else
			handle_word(line, c, sh);
	}
}

t_token	*tokenizer(char *line, t_mini_sh *sh)
{
	int			checker;
	t_tokctx	c;

	c.i = 0;
	c.head = NULL;
	c.last = NULL;
	if (line[0] == '|')
	{
		ft_put_error("minishell", "syntax error near unexpected token `|'");
		sh->last_status = 2;
		return (NULL);
	}
	checker = check_quotes(line);
	if (checker == 0)
		ft_build_tokens(line, &c, sh);
	else if (checker == 1)
		printf("Unclosed quotes\n");
	return (c.head);
}
