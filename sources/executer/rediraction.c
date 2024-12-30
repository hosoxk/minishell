/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediraction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:59:33 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 13:21:06 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	open_file(char *file, t_file_mode mode)
// {
// 	int	ret;

// 	if (mode == READ_ONLY)
// 		ret = open(file, O_RDONLY, 0777);
// 	if (mode == WRITE_ONLY)
// 		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 	if (mode == WRITE_APPEND)
// 		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
// 	if (ret == -1)
// 	{
// 		perror("Perror");
// 		ft_putendl_fd(file, STDERR_FILENO);
// 	}
// 	return (ret);
// }