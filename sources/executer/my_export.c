/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:52:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/02 12:10:42 by kvanden-         ###   ########.fr       */
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

static int get_location(char ***env, char **argv)
{
	char	*name;
    int     index;

	name = ft_substr(argv[1], 0, ft_strchr(argv[1], '=') - argv[1]);
	if (!name)
		return (-1);
    index = getenv_index(name, *env);
	free(name);
    if (index >= 0)
        return (index);   
    index = getenv_index("NULL", *env);
    if (index >= 0)
        return (index);   
    index = ft_tab_len(*env);
	*env = expent_env(*env); //////////////
	return (index);
}

void	export(char ***env, char **argv)
{
	int index;

	index = get_location(env, argv);
	if (index >= 0)
	{
		free((*env)[index]);
		(*env)[index] = ft_strdup(argv[1]);
	}
	else
		ft_putendl_fd("iets fouts export", STDERR_FILENO); ////
}
