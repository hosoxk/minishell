#include "../minishell.h"

static void	handle_sigint(int sig)
{
	(void)sig;

	g_exit_status = 130;
	printf("\n"); // new line for clean prompt ?? // TODO
	rl_on_new_line(); // resets new line to new rule
	rl_replace_line("", 0); // clean current line
	rl_redisplay(); // shows new line
}

static void	disable_signal_chars(void)
{
	struct termios	term;

	// get current terminal settings
	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		print_error("tcgetattr");
		exit(1);
	}
	// disable signal-generating control characters
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	// apply new settings
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		print_error("tcsetattr");
		exit(1);
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN; // ignore SIGQUIT
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
