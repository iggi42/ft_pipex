/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:40:40 by fkruger           #+#    #+#             */
/*   Updated: 2026/03/27 14:40:41 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void	close_pipe(int *pipe)
{
	close(pipe[0]);
	close(pipe[1]);
}

void	mv_fd(int from, int to)
{
	dup2(from, to);
	close(from);
}

pid_t	ft_fork(void)
{
	pid_t	result;

	errno = 0;
	result = fork();
	if (result < 0)
		error_out(EXIT_FAILURE, NULL, errno);
	return (result);
}
