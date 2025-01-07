/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:20 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/07 09:38:29 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    do_pipe(t_ast *ast_root, char ***env, pid_t *pids)
{
    pid_t   pid;
    int     p_fd[2];
    int     fd_in;
    int     fd_out;

    fd_in = dup(STDIN_FILENO);
    fd_out = dup(STDOUT_FILENO);
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
    execute(ast_root->right, env, pids, true);
    dup2(fd_in, STDIN_FILENO);
    dup2(fd_out, STDOUT_FILENO);
}
