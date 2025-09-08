/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executions_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:36:18 by nalesso           #+#    #+#             */
/*   Updated: 2025/09/08 20:04:22 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	preflight_path(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		ft_put_error(path, strerror(errno));
		if (errno == ENOENT || errno == ENOTDIR)
			return (127);
		else
			return (126);
	}
	if (S_ISDIR(st.st_mode))
		return (ft_put_error(path, "Is a directory"), 126);
	if (access(path, X_OK == -1))
		return (ft_put_error(path, strerror(errno)), 126);
	return (0);
}

static char	*join_dir_cmd(char *dir, char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return (res);
}

char	*ft_get_path(t_ast *command, t_mini_sh *sh) // intento de refactor
{
	t_env	*path;
	char	**dirs;
	char	*cand;
	int		i;

	path = ft_getenv(sh->env, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path->value, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		cand = join_dir_cmd(dirs[i], command->args[0]);
		if (cand && access(cand, X_OK) == 0)
			return (ft_free_strs(dirs), cand);
		free(cand);
		i++;
	}
	ft_free_strs(dirs);
	return (NULL);
}

void	ft_put_error(char *prefix, char *msg)
{
	write(2, prefix, ft_strlen(prefix));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
