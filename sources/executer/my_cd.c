/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 11:25:13 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 11:28:48 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_new_dir(char *cwd, char **env, char **argv)
{
	char	*nwd;

	nwd = NULL;
	if (ft_tab_len(argv) > 2)
		return (print_error("cd: too many arguments"), NULL);
	if (argv[1] == NULL)
		nwd = ft_strdup(getenv_stript("HOME", env));
	else if (ft_strcmp(argv[1], ".") == 0)
		nwd = ft_strdup(cwd);
	else if (argv[1][0] == '/')
		nwd = ft_strdup(argv[1]);
	else if (ft_strcmp(argv[1], "..") == 0)
		nwd = ft_substr(cwd, 0, ft_strrchr(cwd, '/') - cwd);
	else
		nwd = ft_strjoin_multiple(cwd, "/", argv[1], NULL);
	if (!nwd)
		print_error("malloc failed");
	return (nwd);
}

void	my_cd(char **env, char **argv)
{
	char	cwd[PATH_MAX];
	char	*nwd;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return ;
	}
	nwd = get_new_dir(cwd, env, argv);
	if (!nwd)
		return ;
	if (chdir(nwd) != 0)
	{
		perror("cd");
		g_exit_status = 1;
		free(nwd);
		return ;
	}
	free(nwd);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return ;
	}
	update_env("PWD", cwd, env);
}
