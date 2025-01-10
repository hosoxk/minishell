/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 17:20:02 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Valid commands: should reflect the command's return code
 *	Syntax error: 258
 *	Process gets terminated ->SIGINT: 130
 *	Nonexistent command: 127
 */

volatile int	g_exit_status = 0;

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
	{
		g_exit_status = 1;
		return (print_error("Correct usage: ./minishell"), false);
	}
	if (!envp)
		return (print_error("Failure locating envp"), false);
	return (true);
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
		{
			cur_red->left = cur_red->right;
			cur_red->right = NULL;
			cur_red = next_red;
			next_red = cur_red->right;
		}
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

static void	init_minishell(char **line, t_token **token_list, t_token **temp, t_ast **ast_root)
{
	*line = NULL;
	*token_list = NULL;
	*temp = NULL;
	*ast_root = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**env;
	t_token	*token_list;
	t_token	*temp;
	t_ast	*ast_root;

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
		return (print_error("Non-interactive mode is not supported"), 1);
	while (1)
	{
		init_minishell(&line, &token_list, &temp, &ast_root);
		if (!(line = handle_line(env)))
		{
			printf("exit\n");
			free_program(token_list, ast_root, env);
			exit(g_exit_status);
		}
		if (ft_strcmp(line, "exit") == 0)
			return (free_program(token_list, ast_root, env), 0);
		if (!lexer(line, &token_list))
			return (free_program(token_list, ast_root, env), g_exit_status);
		free(line);
		//print_tokens(token_list);
		kobe_expander(token_list, env);
		temp = token_list;
		if (validate_token_sequence(token_list))
		{
			if ((ast_root = parse_ast(&temp)))
			{
				move_cmds(&ast_root);
				set_root_ast(ast_root, ast_root);
				//	printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
				//	print_ast(ast_root, 0);
				//	printf(BOLD_MAGENTA"\noutput:\n"RESET);
				free_token_list(&token_list);
				executor(ast_root, &env);
				//	printf(BOLD_MAGENTA"end output\n"RESET);
				free_ast(ast_root);
			}
		}
		else
			free_program(token_list, ast_root, NULL);
	}
	return (g_exit_status);
}
