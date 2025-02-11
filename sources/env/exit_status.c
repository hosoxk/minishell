/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 09:39:05 by kvanden-          #+#    #+#             */
/*   Updated: 2025/02/10 11:12:39 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_exit_status(char **env)
{
	int	code;

	code = 0 + *getenv_stript("?", env);
	return (code);
}

bool	set_exit_status(int exit_status, char **env)
{
	int	index;

	index = get_location("?", &env);
	if (index < 0)
		return (false);
	env[index][2] = exit_status;
	return (true);
}

int	get_exit_status_and_free(char **env)
{
	int	exit_status;

	exit_status = get_exit_status(env);
	ft_free_tab(env);
	return (exit_status);
}
