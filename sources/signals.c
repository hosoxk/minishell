/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:46:05 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/20 18:06:37 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	handle_sigint_in_cmd(int sig)
{
	(void)sig;
	// printf("\n");
}

void handle_sigint_here(int sig)
{
    (void)sig;
    g_exit_status = 130;
    rl_on_new_line();
    write(1, "\n", 1);
    //rl_replace_line("", 0);
	close(STDIN_FILENO);
}

/*
static void sigint_handler(int sig)
{
    (void)sig;
    g_exit_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
}
*/

/*	Process gets terminated -> exit status = 130
 *	Prints new line to terminal
 */

void	handle_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*	Get current terminal settings with tcgetattr,
 *	_POSIX_VDISABLE disables signal-generating control characters
 *	Apply new settings to terminal
 */

static void	disable_signal_chars(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		print_error("tcgetattr");
		exit(1);
	}
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		print_error("tcsetattr");
		exit(1);
	}
}

/*	Handles signals SIGINT and SIGQUIT
 *	SIGQUIT gets ignored by the handler SIG_IGN
 *	Disables signals that come from keyboard
 */

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		print_error("Failed to handle SIGINT");
		exit (1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		print_error("Failed to ignore SIGQUIT");
		exit (1);
	}
	disable_signal_chars();
}
