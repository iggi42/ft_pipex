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

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Definition of O_* constants */
#include <unistd.h>

#include "pipex.h"
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

typedef enum {
	no_error = 0,
	arg0_not_found,
	arg0_not_executable
} t_pipex_err ;

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

	fd = open(outfile_path, O_CREAT |  O_TRUNC | O_WRONLY, 0644);
	// ft_printf("joa lol");
	if (fd > 0)
		return (fd);
	error_out(outfile_path);
	return (-1);
}

static void	child_exec(char *cmd)
{
	char	**argv;
	char	*path;

	argv = ft_split(cmd, ' ');
	path = ft_os_search_path(argv[0], __environ);
	if (path)
		execve(path, argv, __environ);
	else
		path = argv[0];
	(ft_arr_each((t_arr) argv, free), free(argv));
	error_out2("not found", path);
}

void mv_fd(int from, int to)
{
	dup2(from, to);
	close(from);
}

pid_t	start_first_cmd(char *cmd, int out_fd, char *infile_path)
{
	pid_t	pid;
	int		in_fd;

	pid = fork();
	if (pid != 0)
		return (close(out_fd), pid);
	in_fd = open_infile(infile_path);
	mv_fd(in_fd, STDIN_FILENO);
	mv_fd(out_fd, STDOUT_FILENO);
	child_exec(cmd);
	return (-1);
}

pid_t	start_last_cmd(char *cmd, int in_fd, char *outfile_path)
{
	pid_t	pid;
	int		out_fd;

	pid = fork();
	if (pid != 0)
		return (close(in_fd), pid);
	out_fd = open_outfile(outfile_path);
	mv_fd(in_fd, STDIN_FILENO);
	mv_fd(out_fd, STDOUT_FILENO);
	child_exec(cmd);
	return (-1);
}

int	wait_pipex(int *pids)
{
	int		res[2];
	size_t	i;

	i = 0;
	while (i < 2)
	{
		waitpid(pids[i], &res[i], 0);
		if(WIFEXITED(res[i]))
			i++;
	}
	return (WEXITSTATUS(res[1]));
}

int	start_pipex(char **argv)
{
	int		fd[2];
	pid_t	pids[2];

	if (pipe2(fd, O_NONBLOCK) == -1)
		return (error_out("pipe"), -1);
	pids[0] = start_first_cmd(argv[1], fd[1], argv[0]);
	pids[1] = start_last_cmd(argv[2], fd[0], argv[3]);
	return (wait_pipex(pids));
}

int	main(int argc, char **argv)
{
	if (argc == 5)
		return (start_pipex(argv + 1));
	else
		ft_printf_fd(STDERR_FILENO, "call with 4 arguments: ./pipex $1 $2 $3 $4\n");
}
