/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:47:19 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 17:34:55 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	my_env(char **env)
{
	while (*env)
	{
		if (ft_strcmp(*env, "NULL") != 0)
			ft_putendl_fd(*env, 1);
		env++;
	}
}
