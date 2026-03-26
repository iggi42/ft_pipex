#include "pipex.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

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

pid_t	ft_fork(void)
{
	pid_t	result;

	errno = 0;
	result = fork();
	if (result < 0)
		error_out(EXIT_FAILURE, NULL, errno);
	return (result);
}
