#include "libft_arr_t.h"
#include "pipex_cmd.h"
#include "pipex_utils.h"
#include <errno.h>
#include <fcntl.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <libft_os.h>
#include <libft_str.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct s_pc
{
	int			pipe[2];
	t_list		*pids;
}				t_pc;

static bool	is_path(char *cmd)
{
	while (cmd != NULL)
	{
		if (*cmd == '/')
			return (true);
		if (*cmd == '\0')
			return (false);
		cmd++;
	}
	return (false);
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

char	*find_exec_file(char *cmd0)
{
	char	*path;

	if (!is_path(cmd0))
		path = ft_os_search_path(cmd0, __environ);
	else
		path = ft_strdup(cmd0);
	if (path == NULL || access(path, F_OK))
	{
		path = ft_strf("%s: command not found", cmd0);
		error_out(EXIT_CMD_NOT_FOUND, path, 0);
	}
	return (path);
}

static void	ft_dup2(int from, int to)
{
	int	result;

	result = dup2(from, to);
	if (result == to)
		return ;
	ft_putendl_fd("dub2 failure", STDERR_FILENO);
	panic(EXIT_FAILURE);
}

void	apply_stdenv(int stdenv[2])
{
	ft_dup2(stdenv[0], STDIN_FILENO);
	ft_dup2(stdenv[1], STDOUT_FILENO);
	close(stdenv[0]);
	close(stdenv[1]);
}

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

int	open_outappfile(char *outfile_path)
{
	int	fd;

	errno = 0;
	fd = open(outfile_path, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (fd > 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}

void	apply_redi_in(char *infile_path)
{
	int	fd;

	fd = open_infile(infile_path);
	ft_dup2(fd, STDIN_FILENO);
	close(fd);
}

void	apply_redi_out(char *outfile_path)
{
	int	fd;

	fd = open_outfile(outfile_path);
	ft_dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	apply_redi_out_app(char *outfile_path)
{
	int	fd;

	fd = open_outfile(outfile_path);
	ft_dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	apply_heredoc(char *end)
{
	(void)end;
	ft_putendl_fd("HEREDOC IS TODO", STDOUT_FILENO);
	panic(42);
}

void	apply_redi(void *content)
{
	t_redi	*apply_me;

	if (content == NULL)
		return ;
	apply_me = content;
	if (apply_me->kind == IN)
		apply_redi_in(apply_me->target);
	else if (apply_me->kind == OUT)
		apply_redi_out(apply_me->target);
	else if (apply_me->kind == OUT_APPEND)
		apply_redi_out_app(apply_me->target);
	else if (apply_me->kind == HERE_DOC)
		apply_heredoc(apply_me->target);
}

// this _never_ returns
static void	exec_cmd(t_cmd cmd, int stdenv[2])
{
	char	*path;

	apply_stdenv(stdenv);
	ft_lstiter(cmd.reds, apply_redi);
	path = find_exec_file(cmd.argv[0]);
	execve(path, cmd.argv, __environ);
	error_out(EXIT_NO_EXEC_PERM, path, errno);
}

static void	add_pid(t_list **pids, pid_t pid)
{
	t_list	*new_el;
	pid_t	*new_content;

	new_content = my_malloc(sizeof(pid_t));
	*new_content = pid;
	new_el = ft_lstnew(new_content);
	ft_lstadd_front(pids, new_el);
}

static void	ft_pipe(int **new_pipe)
{
	if (pipe(*new_pipe) == 0)
		return ;
	error_out(EXIT_FAILURE, NULL, errno);
}

static void	*exec_pipe(void *acc, t_arr_el cmd)
{
	int		fr;
	int		out_pipe[2];
	int		stdenv[2];
	t_pc	*pc;

	pc = acc;
	fr = fork();
	ft_pipe((int **)&out_pipe);
	stdenv[0] = pc->pipe[1];
	stdenv[1] = out_pipe[1];
	if (fr == 0)
	{
		close(out_pipe[0]);
		exec_cmd(*(t_cmd *)cmd, stdenv);
	}
	close(stdenv[0]);
	close(stdenv[1]);
	pc->pipe[1] = out_pipe[0];
	add_pid(&pc->pids, fr);
	return (acc);
}

static t_byte	wait_pipe(t_list pids)
{
	int	result;

	result = ft_wait(*(pid_t *)(pids.content));
	if (pids.next)
		return (wait_pipe(*pids.next));
	else
		return (result);
}

// not defined for an empty pipe, needs at least 1 element!
t_byte	pipex_exec_pipe(t_pipe *full_pipe)
{
	t_pc	pipe_cache;

	ft_arr_fold((t_arr)full_pipe, exec_pipe, &pipe_cache);
	if (pipe_cache.pids)
		return (wait_pipe(*pipe_cache.pids));
	return (-1);
}
