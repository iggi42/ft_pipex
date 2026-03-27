/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:40:45 by fkruger           #+#    #+#             */
/*   Updated: 2026/03/27 14:40:46 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_os.h>
#include <libft_str.h>
#include <stdlib.h>
#include <unistd.h>

#define EXIT_CMD_NOT_FOUND 127
#define EXIT_NO_EXEC_PERM 126

static void	cleanup_child(char **argv, char *path)
{
	ft_arr_each((t_arr)argv, free);
	free(argv);
	free(path);
}

static int	child_exec(char *cmd)
{
	char	**argv;
	char	*path;

	argv = ft_split(cmd, ' ');
	if (argv == NULL)
		error_out(EXIT_FAILURE, NULL, errno, false);
	if (argv[0] == NULL)
		path = ft_strdup("");
	else if (argv[0][0] == '/' || ft_strncmp("./", argv[0], 2) == 0)
		path = ft_strdup(argv[0]);
	else
		path = ft_os_search_path(argv[0], __environ);
	if (access(path, F_OK) != 0)
	{
		cleanup_child(argv, NULL);
		error_out(EXIT_CMD_NOT_FOUND, path, errno, true);
	}
	execve(path, argv, __environ);
	cleanup_child(argv, NULL);
	error_out(EXIT_NO_EXEC_PERM, path, errno, true);
	return (-1);
}

pid_t	start_first_cmd(char *cmd, int *pipe_fds, char *infile_path)
{
	pid_t	pid;
	int		in_fd;

	pid = ft_fork();
	if (pid != 0)
		return (pid);
	dup2(pipe_fds[1], STDOUT_FILENO);
	close_pipe(pipe_fds);
	in_fd = open_infile(infile_path);
	mv_fd(in_fd, STDIN_FILENO);
	child_exec(cmd);
	return (-1);
}

pid_t	start_last_cmd(char *cmd, int *pipe_fds, char *outfile_path)
{
	pid_t	pid;
	int		out_fd;

	pid = ft_fork();
	if (pid != 0)
		return (pid);
	dup2(pipe_fds[0], STDIN_FILENO);
	close_pipe(pipe_fds);
	out_fd = open_outfile(outfile_path);
	mv_fd(out_fd, STDOUT_FILENO);
	return (child_exec(cmd));
}
