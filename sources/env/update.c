/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:29:34 by kvanden-          #+#    #+#             */
/*   Updated: 2025/02/07 12:19:17 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	getenv_index(char *name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	get_location(char *name, char ***env)
{
	int		index;
	char	**new_env;

	index = getenv_index(name, *env);
	if (index >= 0)
		return (index);
	index = getenv_index("NULL", *env);
	if (index >= 0)
		return (index);
	index = ft_tab_len(*env);
	new_env = expent_env(*env);
	if (!new_env)
		return (print_error_status("malloc failed"), -1);
	*env = new_env;
	return (index);
}

bool	update_env(char *name, char *value, char ***env)
{
	int		index;
	char	*new_env;

	index = get_location(name, env);
	if (index < 0)
		return (false);
	new_env = ft_strjoin_multiple(name, "=", value, NULL);
	if (!new_env)
		return (print_error_status("malloc failed"), false);
	free((*env)[index]);
	(*env)[index] = new_env;
	return (true);
}

int	get_exit_status(char **env)
{
	return ((int)*getenv_stript("?", env));
}

bool set_exit_status(int exit_status, char **env)
{
	int		index;

	index = get_location("?", &env);
	if (index < 0)
		return (false);
	env[index][2] = exit_status;
	return (true);
}
