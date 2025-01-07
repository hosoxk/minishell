#include "../minishell.h"

/*	Process gets terminated -> exit status = 130
 *	Prints new line to terminal
 */

static void	handle_sigint(int sig)
{
	(void)sig;

	g_exit_status = 130;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
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
