/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:52:33 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 15:54:52 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_location(char ***env, char **argv)
{
    int i;
    char *name;

    name = ft_substr(argv[1], 0, ft_strchr(argv[1], '=') - argv[1]); ////
    i = 0;
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], name, ft_strlen(name)) == 0 || ft_strcmp((*env)[i], "NULL") == 0)
        {
            free((*env)[i]);
            return ((*env)[i]);
        }
        i++;
    }
    // realock
    return (NULL);
}

void export(char ***env, char **argv)
{
    char *location;

    location = get_location(env, argv);
    if (location)
    {
        location = ft_strdup(argv[1]);
    }
    ft_putendl_fd("export", STDERR_FILENO);
}
