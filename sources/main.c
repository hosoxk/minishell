/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/14 08:43:47 by kvanden-         ###   ########.fr       */
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

static t_ast *get_next(t_ast *cur_red, t_ast *next_red)
{
	cur_red->left = cur_red->right;
	cur_red->right = NULL;
	cur_red = next_red;
	return (cur_red->right);
}

void	move_cmds(t_ast **node)
{
	t_ast	*cmd;
	t_ast	*first_red;
	t_ast	*cur_red;
	t_ast	*next_red;

	if (!(*node))
		return ;
	cmd = *node;
	first_red = cmd->left;
	if (cmd->type == WORD && first_red && (first_red->type >= REDIRECT_IN
			&& first_red->type <= HEREDOC))
	{
		next_red = first_red->right;
		cur_red = first_red;
		while (next_red && next_red->type >= REDIRECT_IN
			&& next_red->type <= HEREDOC)
			next_red = get_next(cur_red, next_red);
		cmd->left = cur_red->left;
		*node = first_red;
		cur_red->left = cmd;
	}
	move_cmds(&(*node)->left);
	move_cmds(&(*node)->right);
} 

/*	Checks parameters when running ./minishell
 *	Checks if STDIN_FILENO is connected to the terminal and sets up signals
 *	Reads input and tokenizes it
 *	After tokenizing, parses the token_list
 *	Gets expanded and executed
 */

void execute_sub_commands(t_token **token_list, char ***env)
{
	t_token *temp;
	t_ast *ast_root;

	kobe_expander(*token_list, *env);
	temp = *token_list;
	print_tokens(&temp);
	if (validate_token_sequence(*token_list))
	{
		if ((ast_root = parse_ast(&temp)))
		{
			move_cmds(&ast_root);
			set_root_ast(ast_root, ast_root);
		//	printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
		//	print_ast(ast_root, 0);
		//	printf(BOLD_MAGENTA"\noutput:\n"RESET);
			free_token_list(token_list);
			executor(ast_root, env);
		//	printf(BOLD_MAGENTA"end output\n"RESET);
			free_ast(ast_root);
		}
	}
	//else
	//	return (1);
}

static bool execute_line(char *line, char ***env)
{
	t_tree	*tree;
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
	execute_token_tree(tree, env);
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
