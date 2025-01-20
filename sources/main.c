/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/20 16:55:47 by kvanden-         ###   ########.fr       */
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
			exit_status(1),
			false);
	if (!envp)
		return (print_error("Failure locating envp"),
			exit_status(1),
			false);
	return (true);
}

/*	Checks parameters when running ./minishell
 *	Checks if STDIN_FILENO is connected to the terminal and sets up signals
 *	Reads input and tokenizes it
 *	After tokenizing, parses the token_list
 *	Gets expanded and executed
 */

//	printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
//	print_ast(ast_root, 0);
//	printf(BOLD_MAGENTA"\noutput:\n"RESET);
//	print_tokens(&tree->token_list); //// 

bool	execute_sub_commands(t_token_tree *tree, char ***env,
		t_token_tree *token_tree_root)
{
	t_ast		*root;
	t_free_data	data;

	if (!expander(tree->token_list, *env))
		return (false);
	g_exit_status = 0;
//	if (!validate_token_sequence(tree->token_list))
//		return (true);
	root = get_ast(tree, &data, token_tree_root);
	if (!root)
		return (false);
	if (!executor(root, env))
	{
		free_ast(root);
		return (false);
	}
	free_ast(root);
	return (true);
}

static bool	execute_line(char *line, char ***env)
{
	t_token_tree	*tree;
	t_token			*token_list;
	int				exit_code;

	exit_code = g_exit_status;
	g_exit_status = 0;
	token_list = NULL;
	if (!lexer(line, &token_list))
		return (free(line), free_token_list(&token_list),
			false);
	free(line);
	tree = NULL;
	build_token_tree(&tree, token_list);
	if (g_exit_status)
		return (free_token_tree(tree), false);
	g_exit_status = exit_code;
	if (!execute_token_tree(tree, env, tree))
		return (free_token_tree(tree), false);
	free_token_tree(tree);
	return (true);
}

static bool is_exit(char *line)
{
	char **list;
	int exit_code;

	list = ft_split(line, ' ');
	if (!list)
		return (false);
	if (ft_strcmp(list[0], "exit") != 0)
		{
			ft_free_tab(list);
			return (false);
		}
	if (list[1] == NULL)
		return (true);

	exit_code = ft_atoi(list[1]);
	if (exit_code == 0 && ft_strcmp(list[1], "0") != 0)
	{
		exit_code = 2;
		print_error("exit: numeric argument required");
	}
	ft_free_tab(list);
	g_exit_status = exit_code % 256;
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env;

	(void)argv;
	if (!check_input(argc, envp))
		return (1);
	env = create_env(envp);
	if (!env)
		return (1);
	if (isatty(STDIN_FILENO))
		setup_signals();
	while (1)
	{
		line = get_line(env);
		if (!line)
			return (ft_free_tab(env), rl_clear_history(), 1);
		if (is_exit(line))
			return (free(line), ft_free_tab(env), rl_clear_history(), g_exit_status);
		if (!execute_line(line, &env))
		{
			if (g_exit_status != 258)
				return (ft_free_tab(env), g_exit_status);
		}
	}
	return (g_exit_status);
}
