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
#include <libft_io.h>
#include <libft_os.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_str.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void error_out(const char *msg)
{
	char *str;

	str = ft_strf("%s: %s", FT_APP_NAME, msg);
	perror(str);
	free(str);
	exit(EXIT_FAILURE);
}

int	open_infile(char *infile_path)
{
	int	fd;

	fd = -1;
	// if (access(infile_path, R_OK) == 0)
	fd = open(infile_path, O_RDONLY);
	if (fd > 0)
		return (fd);
	error_out(infile_path);
	return -1;
}

int	open_outfile(char *outfile_path)
{
	int	fd;

	fd = open(outfile_path, O_WRONLY  | O_CREAT, 0600);
	if (fd > -1)
		return (fd);
	error_out(outfile_path);
	return -1;
}

void	connect_pipes(t_pipex *p)
{
	(void) p;
}

void	do_pipex(char **s_args, char **envp)
{
	t_pipex *data = pipex_data();
	data->fds[0] = open_infile(s_args[0]);
	data->fds[3] = open_outfile(s_args[3]);
	if (pipe(&data->fds[1]))
		return pipex_cleanup();
	ft_printf("[%d, %d, %d, %d]\n", data->fds[0], data->fds[1], data->fds[2], data->fds[3]);
	data->cmds[0] = ft_spawn_cmd(s_args[1], envp, data->fds, pipex_cleanup);
	if (data->cmds[0] > 0)
		data->cmds[1] = ft_spawn_cmd(s_args[2], envp, &(data->fds[2]), pipex_cleanup);
}

int	main(int argc, char **argv, char *envp[])
{
	if (argc == 5)
		do_pipex(argv + 1, envp);
	else
		ft_printf_fd(STDERR_FILENO, "call with 4 arguments\n");
}
