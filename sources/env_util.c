/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:22:51 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 09:38:16 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(char *name, char *value, char **env)
{
	int index;

	index = getenv_index(name, env);
	if (index >= 0)
	{
		free(env[index]);
		env[index] = ft_strjoin_multiple(name, "=", value, NULL);
	}
	else
		ft_putendl_fd("iets fouts update env", STDERR_FILENO); ////
}

int	getenv_index(char *name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*getenv_stript(char *name, char **env)
{
	int	i;

	i = getenv_index(name, env);
	if (i != -1)
		return (env[i] + ft_strlen(name) + 1);
	return (NULL);
}

char	*get_env_value(t_token *token, int index, int len, char **env)
{
	char	*env_name;
	char	*env_value;

	env_name = ft_strndup(&token->value[index], len);
	if (!env_name)
		return (NULL);
	env_value = getenv_stript(env_name, env);
	free(env_name);
	return (env_value);
}
