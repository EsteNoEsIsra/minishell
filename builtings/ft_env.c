/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:52:56 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/26 12:30:13 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sort_env(char **env_arr)
{
	int		i;
	int		swapped;
	int		len;
	char	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (env_arr[i + 1])
		{
			len = ft_strlen(env_arr[i]) + 1;
			if (ft_strncmp(env_arr[i], env_arr[i + 1], len) > 0)
			{
				tmp = env_arr[i];
				env_arr[i] = env_arr[i + 1];
				env_arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	ft_env(t_env *head, char **args, t_mini_sh *sh)
{
	if (args[1])
	{
		ft_put_error("env: No such file or directory", args[1]);
		sh->last_status = 127;
		return ;
	}
	while (head)
	{
		if (head->value)
		{
			write(STDOUT_FILENO, head->name, ft_strlen(head->name));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, head->value, ft_strlen(head->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		head = head->next;
	}
	sh->last_status = 0;
}
