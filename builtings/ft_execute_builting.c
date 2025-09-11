/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_builting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:53:39 by nalesso           #+#    #+#             */
/*   Updated: 2025/08/26 11:53:41 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_execute_builting(t_ast *node, t_mini_sh *sh)
{
	if (!node || node->type != NODE_COMMAND)
		return (0);
	if (!ft_strncmp(node->args[0], "echo", 5))
		return (ft_echo(node->args, sh), 1);
	else if (!ft_strncmp(node->args[0], "cd", 3))
		return (ft_cd(node->args, sh), 1);
	else if (!ft_strncmp(node->args[0], "pwd", 4))
		return (ft_pwd(sh), 1);
	else if (!ft_strncmp(node->args[0], "export", 7))
		return (ft_export(sh->env, node->args, sh), 1);
	else if (!ft_strncmp(node->args[0], "unset", 6))
		return (ft_unset(&(sh->env), node->args, sh), 1);
	else if (!ft_strncmp(node->args[0], "env", 4))
		return (ft_env(sh->env, node->args, sh), 1);
	else if (!ft_strncmp(node->args[0], "exit", 5))
		return (ft_bexit(node->args, sh), 1);
	return (0);
}
