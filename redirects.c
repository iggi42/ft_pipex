#include "pipex.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int	open_infile(char *infile_path)
{
	int	fd;

	errno = 0;
	fd = open(infile_path, O_RDONLY);
	if (fd > 0)
		return (fd);
	error_out(EXIT_FAILURE, infile_path, errno);
	return (-1);
}

int	open_outfile(char *outfile_path)
{
	int	fd;

	errno = 0;
	fd = open(outfile_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd > 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}
