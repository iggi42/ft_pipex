#include "pipex.h"
#include <fcntl.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_str.h>
#include <stdlib.h>
#include <unistd.h>

pid_t	spawn_cmd(char *cmd, char *const *envp)
{
	char	**cmd_ar;
	char	*path;
	pid_t	result;

	cmd_ar = ft_split(cmd, ' ');
	path = cmd_ar[0];
	result = fork();
	if (result > 0)
	{
		execve(path, cmd_ar, envp);
		exit(-1);
	}
	return (result);
}

void	parse_arg_infile(t_pipex_args *arg, char *infile_path)
{
	int	fd;

	fd = -1;
	if (access(infile_path, R_OK) == 0)
		fd = open(infile_path, O_RDONLY);
	arg->in_fd = fd;
}

void	parse_arg_cmd(pid_t *result, char *cmd_line, char *const *envp)
{
	ft_printf("cmd line: [%s]\n", cmd_line);
	*result = spawn_cmd(cmd_line, envp);
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

t_pipex_args	*parse_args(char **args, char **envp)
{
	t_pipex_args	*result;

	result = (t_pipex_args *)malloc(sizeof(t_pipex_args));
	result->errors = NULL;
	if (result == NULL)
		return (NULL);
	parse_arg_infile(result, args[0]);
	parse_arg_cmd(result->cmds, args[1], envp);
	parse_arg_cmd(result->cmds + 1, args[2], envp);
	parse_arg_outfile(result, args[3]);
	return (result);
}

// TODO print errors in here?
bool	is_args_valid(t_pipex_args *args)
{
	return (args->errors == NULL);
}

void	connect_pipes(t_pipex_args *args)
{
	(void)args;
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
