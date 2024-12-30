/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:54:12 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 15:26:15 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unset(char ***env, char **argv)
{
    int i;
    
    i = 0;
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], argv[1], ft_strlen(argv[1])) == 0)
        {
            free((*env)[i]);
            (*env)[i] = ft_strdup("NULL"); /////////////
        }
        i++;
    }
}
