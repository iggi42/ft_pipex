#include "pipex.h"
#include <fcntl.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_str.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	open_infile(char *infile_path)
{
	int	fd;

	fd = -1;
	if (access(infile_path, R_OK) == 0)
		fd = open(infile_path, O_RDONLY);
	perror("joa gell");
	return (fd);
}

int	open_outfile(char *outfile_path)
{
	int	fd;

	fd = -1;
	if (access(outfile_path, W_OK) == 0)
		fd = open(outfile_path, O_WRONLY);
	if (access(outfile_path, F_OK) != 0)
		fd = open(outfile_path, O_WRONLY | O_CREAT, 0600); // TODO what perms???
	return (fd);
}

void	connect_pipes(t_pipex *p)
{
	(void) p;
}

void	do_pipex(char **s_args, char **envp)
{
	(void) envp;

	open_infile(s_args[0]);
}

int	main(int argc, char **argv, char *envp[])
{
	if (argc == 5)
		do_pipex(argv + 1, envp);
	else
		ft_printf("call with 4 arguments\n");
}
