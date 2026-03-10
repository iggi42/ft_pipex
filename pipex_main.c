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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_pipe_end
{
	int		*fds;
	char	*path;
}			t_pipe_end;

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

	fd = open(outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd > 0)
		return (fd);
	error_out(outfile_path);
	return (-1);
}

void	wait_pipex(void)
{
	int	res[2];

	ft_printf("PIDS: [%d, %d]\n", pipex_data()->cmds[0], pipex_data()->cmds[1]);
	waitpid(pipex_data()->cmds[0], &res[0], 0);
	waitpid(pipex_data()->cmds[1], &res[1], 0);
	if (pipex_data()->cmds[1] <= 0)
		return ;
	waitpid(pipex_data()->cmds[1], res + 1, 0);
	close(pipex_data()->fds[2]);
	close(pipex_data()->fds[3]);
}

void	pprint_pipe_end(char *name, t_pipe_end *end)
{
	if (end == NULL)
	{
		ft_printf("pipe end: %s => NULL\n", name);
		return ;
	}
	ft_printf("pipe end: %s\n path: %s\n  fds: (%d, %d, %d, %d)  \n", name,
		end->path, end->fds[0], end->fds[1], end->fds[2], end->fds[3]);
}

static void	ft_close(int *fd)
{
	close(*fd);
	*fd = -1;
}

int	*open_first(void *arg)
{
	t_pipe_end	*start;

	start = arg;
	start->fds[0] = open_infile(start->path);
	ft_close(&start->fds[2]);
	// pprint_pipe_end("first", start);
	return (start->fds);
}

int	*open_last(void *arg)
{
	t_pipe_end	*end;

	end = arg;
	end->fds[3] = open_outfile(end->path);
	ft_close(&end->fds[1]);
	// pprint_pipe_end("last", end);
	return (end->fds + 2);
}

void	*parse_cmd(t_arr_el el)
{
	t_os_exec	*parsed;

	ft_os_exec(cmd0, int *(*setup)(void *), void *setup_arg)

	parsed = ft_os_search_path(char *cmd0, char **envp)
	return (parsed);
}

void	start_pipex(size_t argc, char **args)
{
	t_arr	*parsed_cmds;

	parsed_cmds = ft_arr_nmap((t_arr)(args + 1), argc - 2, parse_cmd);
}

void	start_pipex_old(char **s_args, char **envp)
{
	int			fd[4];
	t_os_exec	*c1;
	t_os_exec	*c2;

	t_pipe_end end, start;
	c1 = ft_os_cmd_parse(s_args[1], envp);
	c2 = ft_os_cmd_parse(s_args[2], envp);
	start.fds = (int *)&fd;
	start.path = s_args[0];
	end.fds = (int *)&fd;
	end.path = s_args[3];
	if (pipe(fd + 1) == -1)
		return (free(c1), free(c2));
	ft_switch(&fd[1], &fd[2]);
	ft_os_exec(c1, open_first, &start);
	ft_os_exec(c2, open_last, &end);
	(free(c1), free(c2));
}

int	main(int argc, char **argv)
{
	if (argc == 5)
		start_pipex(argc - 1, argv + 1);
		// wait_pipex();
	else
		ft_printf_fd(STDERR_FILENO, "call with 4 arguments\n");
}
