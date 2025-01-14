/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/14 12:01:38 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Succesfull execution: 0
 *	Syntax errors: 258
 *	Command not found: 127
 *	If command exists but is not executable: 126
 *	Malloc fails: 1
 *	Execution errors: 1
 *	Build-in errors: 1
 *	File not found or permission denied: 1
 *	Process gets terminated -> SIGINT: 130
 *							-> SIGQUIT: 131
 *	Pipeline failure: last command exit_status
 *	Non-interactive unsupported: 1
 */

volatile int	g_exit_status = 0;

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
		return (print_error("Correct usage: ./minishell"),
			exit_status(1), false);
	if (!envp)
		return (print_error("Failure locating envp"),
			exit_status(1), false);
	return (true);
}

/*	Checks parameters when running ./minishell
 *	Checks if STDIN_FILENO is connected to the terminal and sets up signals
 *	Reads input and tokenizes it
 *	After tokenizing, parses the token_list
 *	Gets expanded and executed
 */

void execute_sub_commands(t_token_tree *tree, char ***env, t_token_tree *token_tree_root)
{
	t_ast *root;
	t_free_data data;

	kobe_expander(tree->token_list, *env);
	print_tokens(&tree->token_list);
	if (validate_token_sequence(tree->token_list))
	{
		if ((root = get_tast(tree, &data, token_tree_root)))
		{

		//	printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
		//	print_ast(ast_root, 0);
		//	printf(BOLD_MAGENTA"\noutput:\n"RESET);
			executor(root, env);
		//	printf(BOLD_MAGENTA"end output\n"RESET);
			free_ast(root);
		}
	}
	//else
	//	return (1);
}

static bool execute_line(char *line, char ***env)
{
	t_token_tree	*tree;
	t_token *token_list;
	int exit_code;

	exit_code = g_exit_status;
	g_exit_status = 0;
	token_list = NULL;
	if (!lexer(line, &token_list)) ////
		return (free(line), ft_free_tab(*env), false);
	free(line);
	print_tokens(&token_list);
	tree = NULL;
	build_token_tree(&tree, token_list);
	if (g_exit_status)
		// return (free_token_tree(tree), false);
		return (false);
	g_exit_status = exit_code;
	execute_token_tree(tree, env, tree);
	free_token_tree(tree);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env;

	(void)argv;
	env = envp;
	if (!check_input(argc, envp))
		return (1);
	env = ft_copy_tab(envp);
	if (!env)
		return (print_error("Failure copying envp into env"), 1);
	// update_env("SHLVL", "2", env); ///
	if (isatty(STDIN_FILENO))
		setup_signals();
	else
		return (print_error("Non-interactive mode is not supported"), 1); /////////
	while (1)
	{
		if (!(line = handle_line(env)))
			return (printf("exit\n"), ft_free_tab(env), g_exit_status);
		if (ft_strcmp(line, "exit") == 0)
			return (printf("exit\n"), free(line), ft_free_tab(env), 0);
		if (!execute_line(line, &env))
			return (g_exit_status);
	}
	return (0);
}
