/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/16 17:14:16 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	(void)argv;
	(void)envp;

	if (argc != 1)
		return (printf("fuckyou\n"), 1);
	/*while (*envp)
		printf("%s\n", *(envp++));*/
	while (1)
	{
		line = readline(BOLD_MAGENTA"$minishell: "RESET);
		add_history(line);
		if (!lexer(line)) //TODO
			return (1);
		free (line);
	}
	/*parser(); //TODO
	//expander(); //TODO
	//executor(); //TODO
	*/
	return (0);
}
