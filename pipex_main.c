/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:52:47 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/07 04:52:50 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_os.h>
#include <libft_str.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	do_pipex(char **argv)
{
	int		pipe_fds[2];
	pid_t	pids[2];

	if (pipe(pipe_fds) == -1)
		return (error_out(EXIT_FAILURE, NULL, errno, false), -1);
	pids[0] = start_first_cmd(argv[1], pipe_fds, argv[0]);
	pids[1] = start_last_cmd(argv[2], pipe_fds, argv[3]);
	close_pipe(pipe_fds);
	ft_wait(pids[0]);
	return (ft_wait(pids[1]));
}

int	main(int argc, char **argv)
{
	if (argc == 5)
		return (do_pipex(argv + 1));
	else
		error_out(EXIT_FAILURE, HELP_TEXT, 0, false);
	return (EXIT_FAILURE);
}
