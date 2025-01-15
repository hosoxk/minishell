/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:16:19 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 14:16:19 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
void	save_cmd(char *line)
{
<<<<<<< HEAD
	int		fd;
	time_t	now;
	char	timestamp[20]; // Buffer voor tijdstempel (bijv. "2025-01-15 14:30:00\n")
=======
	static int	fd = -1;
>>>>>>> 7ba8178a21380a28f1ed76ac48dba96031b0a570

	if (!line)
		return ;

	// Open het bestand
	fd = open("minishell_history.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
		return ;

	// Haal de huidige tijd op en formatteer deze
	now = time(NULL);
	if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now))) {
		// Schrijf tijdstempel naar bestand
		dprintf(fd, "[%s] ", timestamp);
	}

	// Schrijf de ingevoerde lijn naar het bestand
	ft_putendl_fd(line, fd);

	// Sluit het bestand
	close(fd);
}

char	*get_shell_input(void)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
	return (ft_strdup(buffer));
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

	// if (!isatty(STDIN_FILENO))
	// 	return (get_shell_input());
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
