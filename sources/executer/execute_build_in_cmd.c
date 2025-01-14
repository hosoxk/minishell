/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_build_in_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:00:45 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/07 12:10:42 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**all_paths;
	char	*path;

	i = -1;
	all_paths = ft_split(getenv_stript("PATH", env), ':');
	while (all_paths && all_paths[++i])
	{
		path = ft_strjoin(all_paths[i], "/");
		exec = ft_strjoin(path, cmd);
		ft_free((void **)&path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(all_paths);
			return (exec);
		}
		ft_free((void **)&exec);
	}
	ft_free_tab(all_paths);
	return (NULL);
}

void	execute_build_in_cmd(char *name, char **argv, char **env)
{
	char	*path;

	if (!name || !argv[0])
	{
		ft_putendl_fd("pipex: command not found: ", STDERR_FILENO);
		exit(127);
	}
	path = get_path(name, env);
	if (path)
	{
		execve(path, argv, env);
		free(path);
	}
	execve(name, argv, env);
	free(path);
}
