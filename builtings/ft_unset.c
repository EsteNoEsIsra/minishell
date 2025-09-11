/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:54:36 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/26 11:54:38 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_remove_env(t_env **head, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *head;
	while (curr)
	{
		if (!ft_strncmp(curr->name, key, ft_strlen(key) + 1))
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = (*head)->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_env **head, char **vars, t_mini_sh *sh)
{
	int	i;

	i = 1;
	while (vars[i])
	{
		ft_remove_env(head, vars[i]);
		i++;
	}
	sh->last_status = 0;
}
