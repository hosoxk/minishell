/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:12:51 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/06 17:47:45 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_terminal_settings(struct termios *orig_termios)
{
	if (tcgetattr(STDIN_FILENO, orig_termios) == -1)
	{
		print_error("tcgetattr error: getting terminal settings");
		exit(1);
	}
}

void	restore_terminal_settings(const struct termios *orig_termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, orig_termios) == -1)
	{
		print_error("tcsetattr error: setting terminal settings");
		exit(1);
	}
}
