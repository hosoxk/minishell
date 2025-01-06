/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:20 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 16:23:51 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    do_pipe(t_ast *ast_root, char ***env, pid_t *pids)
{
    pid_t   pid;
    int     p_fd[2];
    int     original_stdin;
    int     original_stdout;

    // Save original stdin/stdout
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);

    if (pipe(p_fd) == -1)
        exit(1);
    pid = fork();
    if (pid == -1)
        exit(1);
    
    if (!pid)
    {
        close(p_fd[0]);
        dup2(p_fd[1], STDOUT_FILENO);
        close(p_fd[1]);
        execute(ast_root->left, env, pids, false);
    }
    
    set_pid(pids, pid);
    close(p_fd[1]);
    dup2(p_fd[0], STDIN_FILENO);
    close(p_fd[0]);
    execute(ast_root->right, env, pids, true);

    // Restore original stdin/stdout
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}
