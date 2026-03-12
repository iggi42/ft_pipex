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

	fd = open(outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	if (!path)
		error_out(ft_strf("not found: %s", argv[0]));
	execve(path, argv, __environ);
}

pid_t	start_first_cmd(char *cmd, int out_fd, char *infile_path)
{
	pid_t	pid;
	int		in_fd;

	pid = fork();
	if (pid != 0)
		return (close(out_fd), pid);
	in_fd = open_infile(infile_path);
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
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
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
	child_exec(cmd);
	return (-1);
}

int	wait_pipex(int *pids)
{
	int		res[2];
	size_t	i;
	pid_t	wait_pid;

	i = 0;
	while (i < 2)
	{
		wait_pid = waitpid(pids[i], &res[i], 0);
		if(WIFEXITED(res[i]))
			i++;
	}
	return (WEXITSTATUS(res[1]));
}

int	start_pipex(char **argv)
{
	int		fd[2];
	pid_t	pids[2];

	if (pipe(fd) == -1)
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
		ft_printf_fd(STDERR_FILENO, "call with 4 arguments\n");
}
