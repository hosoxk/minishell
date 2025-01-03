/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:52:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/03 13:10:54 by kvanden-         ###   ########.fr       */
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
	char     *is_location;
    int     index;
	
	is_location = ft_strchr(argv[1], '=');
	if (!is_location)
		return (-1); /////////
	name = ft_substr(argv[1], 0,  is_location- argv[1]);
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

static void my_set_var(char ***env, char **argv, int index)
{
	free((*env)[index]);
	if (argv[1][ft_strlen(argv[1]) - 1] != '=')
	{
		(*env)[index] = ft_strdup(argv[1]);	 //////
		return ;
	}
	(*env)[index] = ft_strjoin(argv[1], argv[2]); /// 
}

void	export(char ***env, char **argv)
{
	int index;

	index = get_location(env, argv);
	if (index >= 0)
		my_set_var(env, argv, index);
	else
		ft_putendl_fd("iets fouts export", STDERR_FILENO); ////
}
