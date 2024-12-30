/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/30 15:18:15 by yde-rudd         ###   ########.fr       */
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
	if (ft_strcmp(line, "exit") == 0)
		return (NULL);
	if (*line)
		add_history(line);
	return (line);
}

void parse_token(char *line, t_token *token_list, t_ast *ast_root)
{
	if (!line)
		return ;
	// tokenize input
	lexer(line, &token_list);
	free (line);
	if (validate_token_sequence(token_list))
	{
		if ((ast_root = parse_ast(&token_list)))
		{
			//expander(); //TODO
			//executor(); //TODO
			printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
			print_ast(ast_root, 0);
		}
	}
	//print_tokens(&token_list);
	// parse tokens into AST
	free_ast(ast_root);
	free_token_list(&token_list);
}

void excecute_test(char *line, t_token **token_list, t_ast **ast_root)
{
	if (line)
		parse_token(line, *token_list, *ast_root);
	*token_list = NULL;
	*ast_root = NULL;
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
		if (DEBUG == 0)
			parse_token(line, token_list, ast_root);
		else if (DEBUG == 1)
		{
			if (ft_strcmp("test", line) == 0)
			{
				excecute_test(ft_strdup("ls -lR"), &token_list, &ast_root);
				excecute_test(ft_strdup("echo \"hello\""), &token_list, &ast_root);
			}
			else
				parse_token(line, token_list, ast_root);
		}
	}
	return (0);
}
