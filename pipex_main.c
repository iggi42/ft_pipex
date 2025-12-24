#include "pipex.h"
#include <fcntl.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <stdlib.h>
#include <unistd.h>

void	parse_arg_infile(t_pipex_args *arg, char *infile_path)
{
	int	fd;

	fd = -1;
	if (access(infile_path, R_OK) == 0)
		fd = open(infile_path, O_RDONLY);
	arg->in_fd = fd;
}

void	parse_arg_cmd(char **cmd, char *in_cmd)
{
	*cmd = in_cmd;
}

void	parse_arg_cmds(t_pipex_args *arg, char **cmds, size_t n)
{
	if (n != 2)
		ft_printf_fd(2, "jo, wtf. loook here again\n");
	parse_arg_cmd(&(arg->cmds[0]), cmds[0]);
	parse_arg_cmd(&(arg->cmds[1]), cmds[1]);
}

void	parse_arg_outfile(t_pipex_args *arg, char *outfile_path)
{
	int	fd;

	fd = -1;
	if (access(outfile_path, W_OK) == 0)
		fd = open(outfile_path, O_WRONLY);
	if (access(outfile_path, F_OK) != 0)
		fd = open(outfile_path, O_WRONLY | O_CREAT, 0600); // TODO what perms???
	arg->in_fd = fd;
}

t_pipex_args	*parse_args(char **args)
{
	t_pipex_args	*result;

	result = (t_pipex_args *)malloc(sizeof(t_pipex_args));
	result->errors = NULL;
	if (result == NULL)
		return (NULL);
	result->cmds = ft_calloc(3, sizeof(char *));
	if (result->cmds == NULL)
		return (ft_free(1, &result));
	parse_arg_infile(result, args[0]);
	parse_arg_cmds(result, args + 1, 2);
	parse_arg_outfile(result, args[3]);
	return (result);
}

//TODO print errors in here?
bool	is_args_valid(t_pipex_args *args)
{
	return (ft_lstsize(args->errors) == 0);
}

void	do_pipex(char **s_args)
{
	t_pipex_args	*args;

	args = parse_args(s_args);
	if (is_args_valid(args))
	{
		ft_printf("valid args :)\n");
	}
}

int	main(int argc, char **argv)
{
	if (argc == 5)
		do_pipex(argv + 1);
}
