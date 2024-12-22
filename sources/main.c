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

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
		return (printf(BOLD_RED"Correct usage: ./minishell\n"RESET),
			false);
	if (!envp)
		return (printf(BOLD_RED"Failure locating envp\n"RESET), 
			false);
	/*while (*envp)
	 * printf("%s\n", *(envp++));*/
	return (true);
}

static char	*handle_line(void)
{
	char	*line;

	line = readline(BOLD_MAGENTA"$minishell: "RESET);
	if (!line)
		return (false);
	if (*line)
		add_history(line);
	return (line);
}

int	main(int argc, char **envp)
{
	(void)envp;
	char	*line;
	t_token	*token_list;
	t_ast	*ast_root;

	if (!check_input(argc, envp))
		return (1);
	while (1)
	{
		token_list = NULL;
		ast_root = NULL;
		line = NULL;
		// read input
		if (!(line = handle_line()))
			return (1);
		// tokenize input
		lexer(line, &token_list);
		free (line);
		print_tokens(&token_list);
		// parse tokens into AST
		ast_root = parse_ast(&token_list);
		if (ast_root)
		{
			printf(BOLD_RED"\nAbstract Syntax Tree:\n"RESET);
			print_ast(ast_root, 0);
		}
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
