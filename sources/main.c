/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/05 15:19:37 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// heredoc, grep have valgrind errors!

volatile int	g_exit_status = 0;

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
		return (print_error_status("Correct usage: ./minishell"),
			false);
	if (!envp)
		return (print_error_status("Failure locating envp"),
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

bool	execute_token_list(t_token *token_list, char ***env)
{
	t_ast		*root;
	t_free_data	data;
	bool		success;

	if (!expander(token_list, *env))
	{
		free_token_list(&token_list);
		return (false);
	}
	g_exit_status = 0;
	root = get_ast(token_list, &data);
	if (!root)
		return (true);
	signal(SIGINT, handle_sigint_in_cmd);
	success = executor(root, env);
	signal(SIGINT, handle_sigint);
	free_ast(root);
	return (success);
}

static bool	execute_line(char *line, char ***env)
{
	t_token			*token_list;

	token_list = NULL;
	if (!lexer(line, &token_list))
		return (free(line), free_token_list(&token_list),
			false);
	free(line);
	if (!validate_token_sequence(token_list))
	{
		free_token_list(&token_list);
		return (true);
	}
	return (execute_token_list(token_list, env));
}

static bool	is_exit(char *line)
{
	char	**list;
	int		exit_code;
	int		i;

	list = ft_split(line, ' ');
	if (!list)
		return (false);
	if (ft_strcmp(list[0], "exit") != 0)
		return (ft_free_tab(list), false);
	if (list[1] == NULL)
		return (ft_free_tab(list), true);
	if (list[2])
		return (print_error("exit: too many arguments"), ft_free_tab(list), false);
	exit_code = ft_atoi(list[1]);
	if (exit_code == 0 && ft_strcmp(list[1], "0") != 0)
	{
		i = 0;
		while (list[1][i])
		{
			if (ft_isdigit(list[1][i]) != 1)
				return (ft_free_tab(list), print_error("exit: one numeric argument required"), false);
			i++;
		}
		exit_code = 2;
	}
	g_exit_status = exit_code % 256;
	return (ft_free_tab(list), true);
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	char			**env;
	struct termios	orig_termios;

	(void)argv;
	save_terminal_settings(&orig_termios);
//	if (!setup_signals())
//		return (restore_terminal_settings(&orig_termios), g_exit_status);
	printf("Original VQUIT: %d\n", orig_termios.c_cc[VQUIT]);
	if (!check_input(argc, envp))
		return (1);
	env = create_env(envp);
	if (!env)
		return (1);
	if (isatty(STDIN_FILENO))
		if (!setup_signals())
			return (ft_free_tab(env), rl_clear_history(),
				restore_terminal_settings(&orig_termios), g_exit_status);
	while (1)
	{
		line = get_line(env);
		if (!line)
			return (ft_free_tab(env), rl_clear_history(),
				restore_terminal_settings(&orig_termios), g_exit_status);
		if (is_exit(line))
			return (free(line), ft_free_tab(env), rl_clear_history(),
				restore_terminal_settings(&orig_termios), g_exit_status);
		if (!execute_line(line, &env))
			if (g_exit_status != 258)
				return (ft_free_tab(env), restore_terminal_settings(&orig_termios),
					g_exit_status);
	}
	restore_terminal_settings(&orig_termios);
	return (g_exit_status);
}
