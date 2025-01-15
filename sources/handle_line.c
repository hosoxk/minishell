/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:30:26 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/15 14:30:26 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void	save_cmd(char *line)
{
	int		fd;
	time_t	now;
	char	timestamp[20];

	if (!line)
		return ;
	fd = open("minishell_history.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
		return ;
	now = time(NULL);
	if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
			localtime(&now)))
		dprintf(fd, "[%s] ", timestamp);
	ft_putendl_fd(line, fd);
	close(fd);
}

char	*get_shell_input(void)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	line[ft_strlen(line) - 1] = '\0';
	return (line);
}

static char	*get_prompt(char **env)
{
	char	*prompt;
	char	*phat;
	char	*last_slash;
	char	*dir_name;

	phat = getenv_stript("PWD", env);
	if (!phat)
		return (NULL);
	last_slash = ft_strrchr(phat, '/');
	if (!last_slash)
		last_slash = phat;
	dir_name = ft_substr(phat, 1 + (last_slash - phat), ft_strlen(last_slash
				+ 1));
	if (!dir_name)
		return (NULL);
	if (g_exit_status != 0)
		prompt = ft_strjoin_multiple(BOLD_RED, "➜  ", BOLD_BLUE, dir_name,
				BOLD_MAGENTA, "$minishell: ", RESET, NULL);
	else
		prompt = ft_strjoin_multiple(BOLD_GREEN, "➜  ", BOLD_BLUE, dir_name,
				BOLD_MAGENTA, "$minishell ", RESET, NULL);
	free(dir_name);
	if (!prompt)
		return (NULL);
	return (prompt);
}

char	*get_line(char **env)
{
	char	*line;
	char	*promt;

	if (!isatty(STDIN_FILENO))
		return (get_shell_input());
	promt = get_prompt(env);
	if (!promt)
		return (print_error("Failure getting prompt"), NULL);
	line = readline(promt);
	free(promt);
	if (!line)
		return (print_error("Failure getting line"), NULL);
	if (*line)
		add_history(line);
	save_cmd(line); //////////
	return (line);
}
