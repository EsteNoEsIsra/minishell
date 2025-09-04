#include "includes/minishell.h"

t_token	*new_token(t_token_type type, char	*value)
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
	int	start;

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

void	handle_pipe(int *i, t_token **last)
{
	t_token	*tok;

	if (!(*last))
		return ;
	tok = new_token(TOKEN_PIPE, ft_strdup("|"));
	(*last)->next = tok;
	*last = tok;
	(*i)++;
}

void	handle_redirection(char *line, int *i, t_token **head, t_token **last)
{
	t_token	*tok;
	
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		tok = new_token(TOKEN_HEREDOC, ft_strdup("<<"));
		(*i)++;
	}
	else if (line[*i] == '<')
		tok = new_token(TOKEN_REDIR_IN, ft_strdup("<"));
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		tok = new_token(TOKEN_REDIR_APPEND, ft_strdup(">>"));
		(*i)++;
	}
	else
		tok = new_token(TOKEN_REDIR_OUT, ft_strdup(">"));
	if (!*head)
		*head = tok;	
	else
		(*last)->next = tok;
	*last = tok;
	(*i)++;
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

void	handle_word(char *line, int *i, t_token **head, t_token **last, t_mini_sh *sh)
{
	char	*word;
	t_token	*tok;

	word = ft_build_word(line, i, sh);
	if (!word || !*word)
		return (free(word));
	tok = new_token(TOKEN_WORD, word);
	if (!*head)
		*head = tok;
	else if (*last)
		(*last)->next = tok;
	*last = tok;
}

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

void	ft_build_tokens(char *line, t_token **head, t_mini_sh *sh)
{
	int		i;
	t_token	*last;

	i = 0;
	last = NULL;
        while (line[i])
	{
		if (line[i] == '|')
			handle_pipe(&i, &last);
		else if (line[i] == '>' || line[i] == '<')
			handle_redirection(line, &i, head, &last);
		else if (line[i] == ' ')
			i++;
		else
			handle_word(line, &i, head, &last, sh);
	}
}

t_token	*tokenizer(char *line, t_mini_sh *sh)
{
	int		checker;
	t_token	*head;

	head = NULL;
	if (line[0] == '|')
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (NULL);
	}
	checker = check_quotes(line);
	if (checker == 0)
		ft_build_tokens(line, &head, sh);
	else if (checker == 1)
		printf("Unclosed quotes\n");
	return (head);
}
