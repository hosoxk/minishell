/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/18 18:31:17 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*token_list;
	t_ast	*ast_root;
	(void)argv;
	(void)envp;

	if (argc != 1)
		return (printf(BOLD_RED"Correct usage: ./minishell\n"RESET), 1);
	if (!envp)
		return (printf("Failure locating envp\n"), 1);
	/*while (*envp)
	 * printf("%s\n", *(envp++));*/
	while (1)
	{
		token_list = NULL;
		ast_root = NULL;
		line = readline(BOLD_MAGENTA"$minishell: "RESET);
		if (!line)
			break ; // Exit on EOF
		if (*line)
			add_history(line);
		// tokenize input
		lexer(line, &token_list);
		print_tokens(&token_list);
		// parse tokens into AST
		ast_root = parse_ast(&token_list);
		if (ast_root)
		{
			printf(BOLD_RED"\nAbstract Syntax Tree:\n"RESET);
			print_ast(ast_root, 0);
		}
		free (line);
		free_ast(ast_root);
		free_token_list(&token_list);
		/*if (parser()) //TODO
		 * {
			//expander(); //TODO
			//executor(); //TODO
		}*/
	}
	return (0);
}
