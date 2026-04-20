/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:06:28 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 15:06:29 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bw.h"
#include "cmd.h"
#include "redi.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <libft_os.h>
#include <libft_str.h>
#include <string.h>
#include <unistd.h>

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

void	apply_stdenv(int stdenv[2])
{
	ft_dup2(stdenv[R], STDIN_FILENO);
	ft_dup2(stdenv[W], STDOUT_FILENO);
}

static void	my_apply_redi(void *arg)
{
	apply_redi(arg);
}

// this _never_ returns
static void	exec_cmd(t_cmd *cmd, int stdenv[2])
{
	char	*path;

	apply_stdenv(stdenv);
	ft_arr_each((t_arr)(cmd->reds), my_apply_redi);
	path = find_exec_file(cmd->argv[0]);
	ft_bw_cleanup();
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
	ft_lstadd_back(pids, new_el);
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
		stdenv[R] = out_pipe[R];
		if (cmds[1] != NULL)
			stdenv[W] = ft_pipe((int *)out_pipe)[W];
		else
			stdenv[W] = STDOUT_FILENO;
		fr = ft_fork();
		if (fr == 0)
			exec_cmd(cmds[0], stdenv);
		add_pid(&result, fr);
		cmds++;
	}
	return (result);
}

static t_byte	wait_pipe(t_list *pids)
{
	t_byte	result;

	if (pids == NULL)
		return (42);
	result = ft_wait(*(int *)pids->content);
	// ft_printf("result %d\n", result);
	if (pids->next == NULL)
		return (result);
	return (wait_pipe(pids->next));
}

// not defined for an empty pipe, needs at least 1 element!
t_byte	pipex_exec_pipe(t_pipe *full_pipe)
{
	t_list	*pids;

	pids = exec_pipe(full_pipe);
	ft_bw_cleanup();
	return (wait_pipe(pids));
}
