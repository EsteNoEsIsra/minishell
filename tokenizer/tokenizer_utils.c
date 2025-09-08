/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:06:32 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:04:58 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	check_quotes(char *line)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == ';' || line[i] == '\\' || line[i] == '&')
		{
			if (!in_single && !in_double)
			{
				printf("Character not supported: '%c'\n", line[i]);
				return (2);
			}
		}
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

char	*ft_build_word(char *line, int *i, t_mini_sh *sh)
{
	char	*word;
	char	*tmp;
	char	c[2];

	word = ft_strdup("");
	while (line[*i] && line[*i] != '|')
	{
		if (line[*i] == ' ')
			break ;
		else if (line[*i] == '\'' || line[*i] == '"')
			handle_quotes(line, i, &word, sh);
		else if (line[*i] == '$')
			handle_env(line, i, &word, sh);
		else
		{
			c[0] = line[*i];
			c[1] = '\0';
			tmp = word;
			word = ft_strjoin(word, c);
			free(tmp);
			(*i)++;
		}
	}
	return (word);
}

void	expand_last_status(char **word, int *i, t_mini_sh *sh)
{
	char	*tmp;
	char	*status_char;

	status_char = ft_itoa(sh->last_status);
	tmp = ft_strjoin(*word, status_char);
	free(status_char);
	free(*word);
	*word = tmp;
	(*i)++;
}

void	handle_env(char *line, int *i, char **word, t_mini_sh *sh)
{
	char	*name;
	char	*tmp;
	t_env	*env;
	int		start;

	start = ++(*i);
	if (line[*i] == '?')
		return (expand_last_status(word, i, sh));
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	if (start == *i)
	{
		tmp = ft_strjoin(*word, "$");
		free(*word);
		*word = tmp;
		return ;
	}
	name = ft_substr(line, start, *i - start);
	env = ft_getenv(sh->env, name);
	if (!env)
		return (free(name));
	tmp = ft_strjoin(*word, env->value);
	free(*word);
	*word = tmp;
	free(name);
}

void	handle_quotes(char *line, int *i, char **word, t_mini_sh *sh)
{
	char	*tmp;
	char	*seg;
	char	c[2];
	char	quote_char;

	seg = ft_strdup("");
	quote_char = line[(*i)++];
	while (line[*i] && line[*i] != quote_char)
	{
		if (line[*i] == '$' && quote_char == '"')
			handle_env(line, i, &seg, sh);
		else
		{
			c[0] = line[(*i)++];
			c[1] = '\0';
			tmp = seg;
			seg = ft_strjoin(seg, c);
			free(tmp);
		}
	}
	if (line[*i] == quote_char)
		(*i)++;
	tmp = *word;
	*word = ft_strjoin(*word, seg);
	return (free(tmp), free(seg));
}
