/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:59:25 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 18:04:07 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void my_pwd(char **env)
{
    char *pwd;
    pwd = my_getenv("PWD", env);
    ft_putendl_fd(pwd, STDOUT_FILENO);
}
