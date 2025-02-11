/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:46:05 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/06 17:17:25 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_in_cmd(int sig)
{
	(void)sig;
	g_event_val = sig;
	printf("\n");
}

void	handle_sigint_here(int sig)
{
	g_event_val = sig;
	rl_on_new_line();
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_event_val = sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*	Get current terminal settings with tcgetattr,
 *	_POSIX_VDISABLE disables signal-generating control characters
 *	Apply new settings to terminal
 */

void	disable_signal_chars(void)
{
	struct termios	term;
	int				orig_vquit;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		print_error("tcgetattr error: retrieving terminal settings");
		exit(1);
	}
	orig_vquit = term.c_cc[VQUIT];
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		print_error("tcsetattr error: posix vdisable");
		exit(1);
	}
	term.c_cc[VQUIT] = orig_vquit;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		print_error("tcsetattr error: setting original vquit");
		exit(1);
	}
}

/*	Handles signals SIGINT and SIGQUIT
 *	SIGQUIT gets ignored by the handler SIG_IGN
 *	Disables signals that come from keyboard
 */
bool	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		print_error("Failed to handle SIGINT");
		return (false);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		print_error("Failed to ignore SIGQUIT");
		return (false);
	}
	disable_signal_chars();
	return (true);
}
