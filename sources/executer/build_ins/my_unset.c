/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:54:12 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 11:31:42 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	unset(char ***env, char **argv)
{
	int		i;
	char	*temp;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], argv[1], ft_strlen(argv[1])) == 0)
		{
			temp = ft_strdup("NULL");
			if (!temp)
			{
				g_exit_status = 1;
				return (print_error("malloc failed"));
			}
			free((*env)[i]);
			(*env)[i] = temp;
		}
		i++;
	}
}