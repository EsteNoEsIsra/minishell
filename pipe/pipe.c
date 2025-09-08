/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:34:16 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:03:34 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	handle_status(int status, t_mini_sh *sh, int printed)
{
	if (WIFEXITED(status))
	{
		sh->last_status = WEXITSTATUS(status);
		if (sh->last_status == SIGPIPE && !printed)
			ft_put_error("Minishell: ", "Broken pipe");
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		sh->last_status = 128 + WTERMSIG(status);
	}
	else
		sh->last_status = 1;
}

static pid_t	child_left(t_ast *node, t_mini_sh *sh, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(fd[0]);
		close(fd[1]);
		if (node->left->type == NODE_PIPE)
			ft_execute_pipe(node->left, sh);
		else
			ft_execute(node->left, sh);
		ft_free_mini_sh(sh, 1);
		exit(sh->last_status);
	}
	return (pid);
}

static pid_t	child_right(t_ast *node, t_mini_sh *sh, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			exit(1);
		close(fd[1]);
		close(fd[0]);
		if (node->right->type == NODE_PIPE)
			ft_execute_pipe(node->right, sh);
		else
			ft_execute(node->right, sh);
		ft_free_mini_sh(sh, 1);
		exit(sh->last_status);
	}
	return (pid);
}

void	ft_execute_pipe(t_ast *node, t_mini_sh *sh)
{
	int		fd[2];
	int		status;
	int		printed;
	pid_t	pid1;
	pid_t	pid2;

	if (!node || node->type != NODE_PIPE)
		return (ft_put_error("Error", "NODE_PIPE"));
	if (pipe(fd) == -1)
		return (perror("pipe"));
	pid1 = child_left(node, sh, fd);
	pid2 = child_right(node, sh, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status, 0);
	printed = 0;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
	{
		ft_put_error("Minishell: ", "Broken pipe");
		printed = 1;
	}
	waitpid(pid2, &status, 0);
	handle_status(status, sh, printed);
}
