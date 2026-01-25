#include "pipex.h"
#include <fcntl.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_str.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

void	print_env(char *envp[])
{
	size_t	i;

	i = 0;
	while (*(envp + i))
	{
		ft_printf("%i: [%s] \n", i, *(envp + i));
		i++;
	}
}

char	*get_env(char *envp[], char *s)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(s);
	while (*(envp + i))
	{
		if (ft_strncmp(*(envp + i), s, slen) == 0 && *(*(envp + i)
				+ slen) == '=')
			return (*(envp + i) + slen + 1);
		i++;
	}
	return (NULL);
}

char	*find_in_path(char *cmd0, char *envp[])
{
	char	**paths;
	size_t	i;
	char	*full_path;

	if (access(cmd0, X_OK) == 0)
		return (cmd0);
	paths = ft_split(get_env(envp, "PATH"), ':');
	i = 0;
	while (*(paths + i))
	{
		full_path = ft_strf("%s/%s", *(paths + i), cmd0);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

int	open_infile(char *infile_path)
{
	int	fd;

	fd = -1;
	if (access(infile_path, R_OK) == 0)
		fd = open(infile_path, O_RDONLY);
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

// TODO print errors in here?
bool	is_args_valid(t_pipex_args *args)
{
	return (args->errors == NULL);
}

void	connect_pipes(t_pipex_args *args)
{
	int *fds;

	fds = ft_malloc(sizeof(int) * 4);
	pipe(fds);
	pipe(fds + 2);
}

void	do_pipex(char **s_args, char **envp)
{
	t_pipex_args	*args;

	args = parse_args(s_args, envp);

	if (is_args_valid(args))
		connect_pipes(args);
}

int	main(int argc, char **argv, char *envp[])
{
	if (argc == 5)
		do_pipex(argv + 1, envp);
}
