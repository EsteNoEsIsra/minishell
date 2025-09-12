/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:54:09 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/02 16:49:41 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_put_export(t_env *head)
{
	int		i;
	char	**env_arr;
	char	*eq;

	env_arr = ft_env_to_arr(head);
	ft_sort_env(env_arr);
	i = 0;
	while (env_arr[i])
	{
		eq = ft_strchr(env_arr[i], '=');
		write(STDOUT_FILENO, "declare -x ", 11);
		if (eq)
		{
			write(STDOUT_FILENO, env_arr[i], eq - env_arr[i]);
			write(STDOUT_FILENO, "=\"", 2);
			write(STDOUT_FILENO, eq + 1, ft_strlen(eq + 1));
			write(STDOUT_FILENO, "\"", 1);
		}
		else
			write(STDOUT_FILENO, env_arr[i], ft_strlen(env_arr[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	ft_free_strs(env_arr);
}

int	ft_is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
	{
		ft_put_error("export", "not a valid identifier");
		return (0);
	}
	if (ft_isalpha(name[i]) || name[i] == '_')
		i++;
	else
	{
		ft_put_error("export", "not a valid identifier");
		return (0);
	}
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			ft_put_error("export", "not a valid identifier");
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_parse_assignment(char *var, char **key, char **value)
{
	char	*parse;

	*key = NULL;
	*value = NULL;
	parse = ft_strchr(var, '=');
	if (!parse)
	{
		*key = ft_strdup(var);
		if (!*key)
			return (1);
		return (0);
	}
	*key = ft_substr(var, 0, parse - var);
	if (!*key)
		return (1);
	*value = ft_strdup(parse + 1);
	if (!*value)
	{
		free(*key);
		free(value);
		*key = NULL;
		return (1);
	}
	return (0);
}

int	ft_export_process(t_env *head, t_env *final, char *var)
{
	char	*key;
	char	*value;
	t_env	*node;

	if (ft_parse_assignment(var, &key, &value))
		return (1);
	if (!ft_is_valid_name(key))
		return (free(key), free(value), 1);
	node = ft_getenv(head, key);
	if (node)
	{
		if (value)
		{
			free(node->value);
			node->value = ft_strdup(value);
		}
	}
	else
		final->next = ft_new_env(key, value);
	return (0);
}

void	ft_export(t_env *head, char **args, t_mini_sh *sh)
{
	t_env	*final;
	int		i;
	int		ret;

	if (!args || !args[1])
	{
		ft_put_export(head);
		sh->last_status = 0;
		return ;
	}
	final = head;
	while (final && final->next)
		final = final->next;
	sh->last_status = 0;
	i = 1;
	while (args[i])
	{
		ret = ft_export_process(head, final, args[i]);
		if (ret != 0)
			sh->last_status = 1;
		if (final && final->next)
			final = final->next;
		i++;
	}
}
