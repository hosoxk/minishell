/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_build_in_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:00:45 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 17:55:13 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*my_getenv(char *name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0)
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**all_paths;
	char	*path;
	char	**cmd_arr;

	i = -1;
	all_paths = ft_split(my_getenv("PATH", env), ':');
	cmd_arr = ft_split(cmd, ' ');
	while (all_paths && all_paths[++i])
	{
		path = ft_strjoin(all_paths[i], "/");
		exec = ft_strjoin(path, cmd_arr[0]);
		ft_free((void **)&path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(all_paths);
			ft_free_tab(cmd_arr);
			return (exec);
		}
		ft_free((void **)&exec);
	}
	ft_free_tab(all_paths);
	ft_free_tab(cmd_arr);
	return (cmd);
}

static void	clean_up(char *path)
{
	free(path);
	perror("Perror");
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(1);
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
	if (execve(path, argv, env) == -1)
		clean_up(path); // ?
}
