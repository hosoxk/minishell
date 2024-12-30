/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:52:33 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 18:02:34 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**expent_env(char **env)
{
    int     size ;
	int		new_size;
	char	**new_env;

    size = ft_tab_len(env);
	new_size = size * 2;
	new_env = ft_realloc(env, size * sizeof(char *), new_size * sizeof(char *));
	/////////////
	if (!new_env)
		return (NULL);
	while (size < new_size)
	{
		new_env[size] = ft_strdup("NULL"); /////////////
		size++;
	}
	return (new_env);
}

static char	*get_location(char ***env, char **argv)
{
	char	*name;
    char    *location;
    int     i;

	name = ft_substr(argv[1], 0, ft_strchr(argv[1], '=') - argv[1]); ////
    location = my_getenv(name, *env);
	free(name);
    if (location)
        return (location);   
    location = my_getenv("NULL", *env);
    if (location)
        return (location);
    i = ft_tab_len(*env);
	*env = expent_env(*env);
	free((*env)[i]);
	return ((*env)[i]);
}

void	export(char ***env, char **argv)
{
	char	*location;

	location = get_location(env, argv);
	if (location)
	{
		location = ft_strdup(argv[1]);
	}
	else
		ft_putendl_fd("iets fouts export", STDERR_FILENO);
}
