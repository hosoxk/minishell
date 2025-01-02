/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 11:25:13 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/02 16:03:37 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_new_dir(char *cwd, char **env, char **argv)
{
	char	*nwd;

	nwd = NULL;
	if (ft_tab_len(argv) > 2)
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		return (NULL);
	}
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
		perror("malloc");
	return (nwd);
}

void	my_cd(char **env, char **argv)
{
	char	cwd[PATH_MAX];
	char	*nwd;

	if (!getcwd(cwd, sizeof(cwd)))
		perror("getcwd");
	nwd = get_new_dir(cwd, env, argv);
	if (!nwd)
		return ; /// nog ne error geven
    printf("Changing directory to %s\n", nwd);
	if (chdir(nwd) != 0)
	{
		perror("cd");
		return ;
	}
    free(nwd);
    if (!getcwd(cwd, sizeof(cwd)))
		perror("getcwd");
	update_env("PWD", cwd, env);
}
