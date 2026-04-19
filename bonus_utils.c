#include <fcntl.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "pipex_bw.h"

void	panic(int exit_code)
{
	write(2, "[FT_PANIC] (lol)\n", 17);
	ft_m3_cleanup();
	exit(exit_code);
}

void	*my_malloc(size_t size)
{
	void	*result;

	result = ft_malloc(size);
	if (result)
		return (result);
	ft_putendl_fd("OUT OF MEMORY", STDOUT_FILENO);
	panic(EXIT_FAILURE);
	return (NULL);
}

void	print_str_arr(char **ar)
{
	ft_printf("[");
	while (ar != NULL && *ar != NULL)
	{
		ft_printf(" [%p] ", *ar);
		ar++;
	}
	ft_printf("]\n");
}

void	error_out(int exit_code, char *msg, int error_code)
{
	if (msg != NULL)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		if (error_code != 0)
			ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error_code != 0)
		ft_putstr_fd(strerror(error_code), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	panic(exit_code);
}

pid_t	ft_fork(void)
{
	pid_t	result;

	errno = 0;
	result = fork();
	if (result < 0)
		error_out(EXIT_FAILURE, NULL, errno);
	return (result);
}

void ft_close(int fd)
{
	ft_bw_rm(fd);
	close(fd);
}
