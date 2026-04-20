/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:26:51 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 16:26:51 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bw.h"
#include "redi.h"
#include <errno.h>
#include <fcntl.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <libft_str.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	ft_exit(int exit_code)
{
	ft_bw_cleanup();
	ft_m3_cleanup();
	exit(exit_code);
}

void	*my_malloc(size_t size)
{
	void	*result;

	result = ft_malloc(size);
	if (result)
		return (result);
	ft_putendl_fd("\nOUT OF MEMORY\n", STDOUT_FILENO);
	ft_exit(EXIT_FAILURE);
	return (NULL);
}

void	error_out(int exit_code, char *msg, int error_code)
{
	if (msg != NULL)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		if (error_code != 0)
			ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error_code != 0)
		ft_putstr_fd(strerror(error_code), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	ft_exit(exit_code);
}

int	*ft_pipe(int *new_pipe)
{
	if (pipe(new_pipe) == 0)
	{
		ft_bw_add(new_pipe[R]);
		ft_bw_add(new_pipe[W]);
		return (new_pipe);
	}
	error_out(EXIT_FAILURE, "pipe failure", errno);
	return (NULL);
}

pid_t	ft_fork(void)
{
	pid_t	result;

	errno = 0;
	result = fork();
	if (result < 0)
		error_out(EXIT_FAILURE, "fork failure", errno);
	return (result);
}

void	ft_close(int fd)
{
	ft_bw_rm(fd);
	close(fd);
}

void	ft_dup2(int from, int to)
{
	int		result;
	char	*err_msg;

	result = dup2(from, to);
	if (result == to)
		return (ft_bw_add(to));
	err_msg = ft_strf("ft_dup failure [%d => %d]: %s", from, to,
			strerror(errno));
	ft_putendl_fd(err_msg, STDERR_FILENO);
	ft_free(err_msg);
	ft_exit(EXIT_FAILURE);
}

// void	mv_fd(int from, int to)
// {
// 	if (from == to)
// 		return ;
// 	ft_dup2(from, to);
// 	ft_close(from);
// }
