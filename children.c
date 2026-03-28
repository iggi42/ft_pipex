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
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define EXIT_CMD_NOT_FOUND 127
#define EXIT_NO_EXEC_PERM 126

static void	clean_arr(char **argv)
{
	ft_arr_each((t_arr)argv, free);
	free(argv);
}

static bool	is_path(char *cmd)
{
	while (cmd != NULL)
	{
		if (*cmd == '/')
			return (true);
		if (*cmd == '\0')
			return (false);
		cmd++;
	}
	return (false);
}

static int	child_exec(char *cmd)
{
	char	**argv;
	char	*path;

	argv = ft_split(cmd, ' ');
	if (argv == NULL)
		error_out(EXIT_FAILURE, NULL, errno, false);
	if (!is_path(argv[0]))
		path = ft_os_search_path(argv[0], __environ);
	else
		path = ft_strdup(argv[0]);
	if (path == NULL || access(path, F_OK))
	{
		path = (free(path), ft_strf("%s: command not found", argv[0]));
		clean_arr(argv);
		error_out(EXIT_CMD_NOT_FOUND, path, 0, true);
	}
	execve(path, argv, __environ);
	clean_arr(argv);
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
