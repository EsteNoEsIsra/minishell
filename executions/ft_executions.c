/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:39:47 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 18:44:49 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static t_ast	*child_prepare(t_ast *node, t_mini_sh *sh)
{
	node = apply_redirs_and_get_cmd(node);
	if (!node)
	{
		ft_free_mini_sh(sh, 1);
		exit(EXIT_FAILURE);
	}
	if (ft_execute_builting(node, sh))
	{
		ft_free_mini_sh(sh, 1);
		exit(0);
	}
	return (node);
}

static char	*resolve_path(t_ast *node, t_mini_sh *sh)
{
	char	*path;
	int		ec;

	if (ft_strchr(node->args[0], '/'))
	{
		path = ft_strdup(node->args[0]);
		if (!path)
			return (NULL);
		ec = preflight_path(path);
		if (ec)
		{
			free(path);
			ft_free_mini_sh(sh, 1);
			exit(ec);
		}
		return (path);
	}
	path = ft_get_path(node, sh);
	if (!path)
	{
		ft_put_error(node->args[0], "command not found");
		ft_free_mini_sh(sh, 1);
		exit(127);
	}
	return (path);
}

static void	ft_do_exec(char *path, t_ast *node, t_mini_sh *sh)
{
	char	**env;
	int		ec;

	env = ft_env_to_arr(sh->env);
	if (!env)
	{
		free(path);
		ft_free_mini_sh(sh, 1);
		exit(1);
	}
	if (execve(path, node->args, env) == -1)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			ec = 127;
		else
			ec = 126;
		ft_put_error(path, strerror(errno));
		free(path);
		ft_free_strs(env);
		ft_free_mini_sh(sh, 1);
		exit(ec);
	}
}

void	ft_execute(t_ast *node, t_mini_sh *sh)
{
	int		status;
	char	*path;

	if (node->type == NODE_COMMAND && !node->args[0][0])
		return ;
	if (ft_execute_builting(node, sh))
		return ;
	sh->mypid = fork();
	if (sh->mypid == -1)
		return (ft_free_mini_sh(sh, 1));
	if (sh->mypid == 0)
	{
		//ft_setup_own();// poner las señales por defecto en el hijo
		set_signals_interactive();//test----------------------------------
		node = child_prepare(node, sh);
		path = resolve_path(node, sh);
		ft_do_exec(path, node, sh);
	}
	waitpid(sh->mypid, &status, 0);
	handle_status(status, sh, 0);
}
