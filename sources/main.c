/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/17 15:24:40 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*token_list;
	(void)argv;
	(void)envp;

	if (argc != 1)
		return (printf(BOLD_RED"Correct usage: ./minishell\n"RESET), 1);
	if (!envp)
		return (printf("Failure finding envp\n"), 1);
	/*while (*envp)
	 * printf("%s\n", *(envp++));*/
	while (1)
	{
		token_list = NULL;
		line = readline(BOLD_MAGENTA"$minishell: "RESET);
		add_history(line);
		lexer(line, &token_list);
		print_tokens(&token_list);
		/*if (parser()) //TODO
		 * {
			//expander(); //TODO
			//executor(); //TODO
		}*/
		free (line);
		// free_token_list(); //TODO
	}
	return (0);
}
