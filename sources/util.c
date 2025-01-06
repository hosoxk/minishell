/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:43:09 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 11:56:12 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_prompt(char **env)
{
	char	*prompt;
	char	*phat;
	char	*last_slash;
	char	*dir_name;

	phat = getenv_stript("PWD", env);
	if (!phat)
		return ("fack!");
	last_slash = ft_strrchr(phat, '/');
	if (!last_slash)
		last_slash = phat;
	dir_name = ft_substr(phat, 1 + (last_slash - phat), ft_strlen(last_slash
				+ 1));
	if (!dir_name)
		return ("fack!");
	if (g_exit_status != 0)
		prompt = ft_strjoin_multiple(BOLD_RED, "➜  ", BOLD_BLUE, dir_name,
				BOLD_MAGENTA, "$minishell: ", RESET, NULL);
	else
		prompt = ft_strjoin_multiple(BOLD_GREEN, "➜  ", BOLD_BLUE, dir_name,
				BOLD_MAGENTA, "$minishell ", RESET, NULL);
	free(dir_name);
	if (!prompt)
		return ("fack!");
	return (prompt);
}
