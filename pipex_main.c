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

// This doesn't handle envs in the beginning of the line yet
// nor dies it escape space via \ ' or "
pid_t	spawn_cmd(char *cmd, char *const *envp, int pipes[2])
{
	char	**cmd_ar;
	char	*exec_file;
	pid_t	result;

	cmd_ar = ft_split(cmd, ' ');
	exec_file = find_in_path(cmd_ar[0], (char **) envp);
	result = fork();
	if (result == 0)
		execve(exec_file, cmd_ar, envp);
	return (result);
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

void	spawn_pipel(size_t i, t_pipex_args *args, char *cmd_line, char *const *envp)
{
	int *p = args->pipe + i;
	ft_printf("cmd line: [%s]\n", cmd_line);
	pipe(p);
	spawn_cmd(cmd_line, envp, p);
}

t_pipex_args	*parse_args(char **args, char **envp)
{
	t_pipex_args	*result;

	result = (t_pipex_args *)malloc(sizeof(t_pipex_args));
	result->errors = NULL;
	if (result == NULL)
		return (NULL);
	// result->pipe[0] =  parse_arg_infile(result, args[0]);
	parse_arg_cmd(result, args[1], envp);
	parse_arg_cmd(result, args[2], envp);
	// parse_arg_outfile(result, args[3]);
	return (result);
}

// TODO print errors in here?
bool	is_args_valid(t_pipex_args *args)
{
	return (args->errors == NULL);
}

void print_pipex_args(t_pipex_args *args)
{
	ft_printf("in fd: [ %d ]\n", args->in_fd);
	ft_printf("cmd pids: [ %d, %d ]\n", args->cmds[0], args->cmds[1]);
	ft_printf("out fd: [ %d ]\n", args->out_fd);
}

void	connect_pipes(t_pipex_args *args)
{
	int in[2];
	int out[2];
	int cmds[2];

	pipe(in);
	pipe(out);
	pipe(cmds);

	ft_printf("connecting pipes boss [%d]\n",	pipe(args->cmds));
	ft_printf("[%d]\n",	pipe(args->cmds));
	ft_printf("connecting pipes boss [%d]\n",	pipe(args->cmds));

	print_pipex_args(args);
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
