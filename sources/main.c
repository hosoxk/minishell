/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/03 10:56:59 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
		return (print_error("Correct usage: ./minishell"), false);
	if (!envp)
		return (print_error("Failure locating envp"), false);
	return (true);
}

static char	*handle_line(char **env)
{
	char	*line;

	line = readline(get_prompt(env));
	if (!line)
		return (NULL);
	// add command to history
	if (*line)
		add_history(line);
	return (line);
}

static void parse_token(char *line, t_token *token_list, t_ast *ast_root, char ***env)
{
	if (!line)
		return ;
	(void)env;
	// tokenize input
	lexer(line, &token_list);
	free (line);
	expander(token_list, *env);
	print_tokens(&token_list);
	if (validate_token_sequence(token_list))
	{
		// parse tokens into AST
		if ((ast_root = parse_ast(&token_list)))
		{
			executor(ast_root, env);
			printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
			print_ast(ast_root, 0);
		}
	}
}

void excecute_test(char *line, t_token **token_list, t_ast **ast_root, char ***env)
{
	if (line)
		parse_token(line, *token_list, *ast_root, env);
	*token_list = NULL;
	*ast_root = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	*line;
	t_token	*token_list;
	t_ast	*ast_root;
	char	**env;
	
	env = envp;
	if (!check_input(argc, envp))
		return (1);
	env = ft_copy_tab(envp);
	if (!env)
		return (1);
	while (1)
	{
		token_list = NULL;
		ast_root = NULL;
		line = NULL;
		// read input
		if (!(line = handle_line(env)))
			return (1);
		if (ft_strcmp(line, "exit") == 0)
			return (free(line), 0);
		if (DEBUG == 0)
			parse_token(line, token_list, ast_root, &env);
		else if (DEBUG == 1)
		{
			if (ft_strcmp("test", line) == 0)
			{
				excecute_test(ft_strdup("ls -lR"), &token_list, &ast_root, &env);
				excecute_test(ft_strdup("echo \"hello\""), &token_list, &ast_root, &env);
			}
			else
				parse_token(line, token_list, ast_root, &env);
		}
	}
	return (0);
}
