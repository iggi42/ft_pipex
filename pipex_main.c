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

#define _GNU_SOURCE /* See feature_test_macros(7) */
#include "pipex.h"
#include <errno.h>
#include <fcntl.h> /* Definition of O_* constants */
#include <fcntl.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_os.h>
#include <libft_str.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	error_out(const char *msg)
{
	char	*str;

	str = ft_strf("%s: %s", FT_APP_NAME, msg);
	perror(str);
	free(str);
	exit(EXIT_FAILURE);
}

void	error_out2(const char *msg, const char *msg2)
{
	char	*str;

	str = ft_strf("%s: %s: %s", FT_APP_NAME, msg, msg2);
	perror(str);
	free(str);
	exit(EXIT_FAILURE);
}

int	open_infile(char *infile_path)
{
	int	fd;

	fd = open(infile_path, O_RDONLY);
	if (fd > 0)
		return (fd);
	error_out(infile_path);
	return (-1);
}

int	open_outfile(char *outfile_path)
{
	int	fd;

	fd = open(outfile_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd > 0)
		return (fd);
	error_out(outfile_path);
	return (-1);
}

static int	child_exec(char *cmd)
{
	char	**argv;
	char	*path;

	if (ft_strlen(cmd) == 0)
		exit(EXIT_FAILURE);
	argv = ft_split(cmd, ' ');
	if (ft_strncmp("./", argv[0], 2) == 0)
		path = ft_strdup(argv[0]);
	else
		path = ft_os_search_path(argv[0], __environ);
	if (path == NULL)
	{
		ft_arr_each((t_arr)argv, free);
		free(argv);
		free(path);
		perror("command not found");
		exit(127);
	}
	execve(path, argv, __environ);
	if (errno == ENOENT)
		perror(path);
	ft_arr_each((t_arr)argv, free);
	free(argv);
	free(path);
	exit(128);
	return (-1);
}

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

pid_t	start_first_cmd(char *cmd, int *pipe_fds, char *infile_path)
{
	pid_t	pid;
	int		in_fd;

	pid = fork();
	if (pid != 0)
		return (pid);
	in_fd = open_infile(infile_path);
	mv_fd(in_fd, STDIN_FILENO);
	dup2(pipe_fds[1], STDOUT_FILENO);
	close_pipe(pipe_fds);
	child_exec(cmd);
	return (-1);
}

pid_t	start_last_cmd(char *cmd, int *pipe_fds, char *outfile_path)
{
	pid_t	pid;
	int		out_fd;

	pid = fork();
	if (pid != 0)
		return (pid);
	out_fd = open_outfile(outfile_path);
	mv_fd(out_fd, STDOUT_FILENO);
	dup2(pipe_fds[0], STDIN_FILENO);
	close_pipe(pipe_fds);
	return (child_exec(cmd));
}

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

int	start_pipex(char **argv)
{
	int		pipe_fds[2];
	pid_t	pids[2];

	if (pipe(pipe_fds) == -1)
		return (error_out("pipe"), -1);
	pids[0] = start_first_cmd(argv[1], pipe_fds, argv[0]);
	pids[1] = start_last_cmd(argv[2], pipe_fds, argv[3]);
	close_pipe(pipe_fds);
	ft_wait(pids[0]);
	return (ft_wait(pids[1]));
}

int	main(int argc, char **argv)
{
	if (argc == 5)
		return (start_pipex(argv + 1));
	else
		error_out("call with 4 arguments: ./pipex $1 $2 $3 $4\n");
	return (EXIT_SUCCESS);
}
