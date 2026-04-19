#include "libft_arr_t.h"
#include "pipex_bw.h"
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

#include <stdio.h>

enum			e_pipe_end
{
	R = 0,
	W = 1
};

// typedef int		t_fd_pipe[sizeof(enum e_pipe_end)];

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
	ft_putendl_fd(ft_strf("dup failure [%d => %d]: %s", from, to,
			strerror(errno)), STDERR_FILENO);
	panic(EXIT_FAILURE);
}

void	mv_fd(int from, int to)
{
	if (from == to)
		return ;
	dprintf(2,"mv_fd (%d -> %d)\n", from, to);
	ft_dup2(from, to);
	ft_bw_add(to);
	ft_close(from);
}

void	apply_stdenv(int stdenv[2])
{
	mv_fd(stdenv[R], STDIN_FILENO);
	mv_fd(stdenv[W], STDOUT_FILENO);
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
	fd = open(outfile_path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd > 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}

int	open_heredoc(char *end)
{
	(void)end;
	ft_putendl_fd("HEREDOC IS TODO", STDERR_FILENO);
	panic(42);
	return (-1);
}

static int (*get_opn(enum e_redi_kind k))(char *)
{
	if (k == IN)
		return (open_infile);
	if (k == OUT)
		return (open_outfile);
	if (k == OUT_APPEND)
		return (open_outappfile);
	if (k == HERE_DOC)
		return (open_heredoc);
	return (NULL);
}

void	apply_redi(void *content)
{
	t_redi	*apply_me;
	int		fd;

	if (content == NULL)
		return ;
	apply_me = content;
	dprintf(2, "redi kind: %d\n", apply_me->kind);
	fd = (get_opn(apply_me->kind))(apply_me->target);
	ft_bw_add(fd);
	mv_fd(fd, STDIN_FILENO);
}

// this _never_ returns
static void	exec_cmd(t_cmd *cmd, int stdenv[2])
{
	char	*path;

	// ft_printf_fd(2, "in fds: [r: %d, w: %d]\n", stdenv[R], stdenv[W]);
	// ft_printf("lol \n");
	apply_stdenv(stdenv);
	// ft_printf("lol \n");
	// ft_printf_fd(2, "lol %p \n", cmd.reds);
	ft_arr_each((t_arr) (cmd->reds), apply_redi);
	// ft_printf("lol \n");
	path = find_exec_file(cmd->argv[0]);
	// ft_bw_each(dbg_fd);
	// ft_printf_fd(2, "lol [%s] \n", path);
	execve(path, cmd->argv, __environ);
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

static int	*ft_pipe(int *new_pipe)
{
	ft_memset(new_pipe, -1, 2 * sizeof(int));
	if (pipe(new_pipe) == 0)
	{
		ft_bw_add(new_pipe[R]);
		ft_bw_add(new_pipe[W]);
		return (new_pipe);
	}
	error_out(EXIT_FAILURE, NULL, errno);
	return (NULL);
}

// returns a list of pids to wait on
static t_list	*exec_pipe(t_cmd **cmds)
{
	pid_t	fr;
	int		out_pipe[2];
	int		stdenv[2];
	t_list	*result;

	result = NULL;
	out_pipe[R] = STDIN_FILENO;
	while (cmds[0] != NULL)
	{
		// check if a next cmd is in the array to create an pipe to
		stdenv[R] = out_pipe[R];
		if (cmds[1] != NULL)
			stdenv[W] = ft_pipe((int *)out_pipe)[W];
		else // put stdout as stdout for the last cmd
			stdenv[W] = STDOUT_FILENO;
		fr = ft_fork();
		if (fr == 0)
		{
			// if(out_pipe[R] > 2)
			// 	ft_close(out_pipe[R]);
			exec_cmd(*cmds, stdenv);
		}
		if (out_pipe[W] > 2)
			ft_close(out_pipe[W]);
		if (stdenv[R] > 2)
			ft_close(stdenv[R]);
		add_pid(&result, fr);
		cmds++;
	}
	// ft_bw_each(dbg_fd);
	return (result);
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
	t_list	*pids;

	pids = exec_pipe(full_pipe);
	if (pids)
		return (wait_pipe(*pids));
	return (-1);
}
