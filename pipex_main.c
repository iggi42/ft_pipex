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
#include <libft_os.h>
#include <libft_str.h>
#include <stdlib.h>
#include <sys/wait.h>

int	get_exit_code(int res)
{
	if (WIFEXITED(res))
		return (WEXITSTATUS(res));
	if (WIFSIGNALED(res))
		return (128 + WTERMSIG(res));
	return (res);
}

int	ft_wait(pid_t pid)
{
	int		res;
	pid_t	sig_pid;

	sig_pid = -1;
	res = 0;
	while (true)
	{
		sig_pid = waitpid(pid, &res, 0);
		if (sig_pid == pid)
			return (get_exit_code(res));
	}
}

int	do_pipex(char **argv)
{
	int		pipe_fds[2];
	pid_t	pids[2];

	if (pipe(pipe_fds) == -1)
		return (error_out(EXIT_FAILURE, "pipe failed", errno), -1);
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
		error_out(EXIT_FAILURE, "call with 4 arguments: ./pipex $1 $2 $3 $4\n",
			0);
	return (EXIT_FAILURE);
}
