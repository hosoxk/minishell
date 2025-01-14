
#include "minishell.h"

static void	save_cmd(char *line) //////////
{
	static int fd = -1;

	if (!line)
		return ;
	if (fd == -1)
		fd = open("minishell_history.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ft_putendl_fd(line, fd);
}

char	*get_shell_input(void)
{
	char	buffer[4096];
	ssize_t	bytes_read;

    printf("wooow");
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
	return (ft_strdup(buffer));
}

char	*handle_line(char **env)
{
	char	*line;
	char	*promt;

	// if (!isatty(STDIN_FILENO))
	// 	return (get_shell_input());
	promt = get_prompt(env);
	line = readline(promt);
	free(promt);
	if (!line)
		return (NULL);
	if (*line)
		add_history(line);
	save_cmd(line); //////////
	return (line);
}
