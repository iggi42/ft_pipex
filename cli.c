/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 17:53:14 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 17:53:51 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cmd.h"
#include "here_doc.h"
#include "redi.h"
#include "utils.h"
#include <fcntl.h>
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_os.h>
#include <libft_merle.h>
#include <libft_str.h>
#include <stdlib.h>

void	print_help(void)
{
	ft_putendl_fd("computer sagt nein", 2);
}

t_arr_el	my_parse(t_arr_el el)
{
	t_cmd	*result;

	result = my_malloc(sizeof(t_cmd));
	ft_bzero(result, sizeof(t_cmd));
	parse_cmd(el, result);
	return (result);
}

static void	set_redi(t_cmd *cmd, t_redi *redi)
{
	ft_arr_each((t_arr)cmd->reds, ft_free);
	ft_free(cmd->reds);
	cmd->reds = (t_redi **)ft_arr_new(1);
	cmd->reds[0] = redi;
}

static void	build_normal_redis(t_redi *redis, char *inpath, char *outpath)
{
	redis[0].kind = IN;
	redi_set_path(&redis[0], inpath);
	redis[1].kind = OUT;
	redi_set_path(&redis[1], outpath);
}

static void	build_bonus_redis(t_redi *redis, char *hd_limiter, char *outpath)
{
	int here_doc_fd;
	pid_t writer_pid;

	here_doc_fd = 
	redis[0].kind = IN;
	redi_set_fd(&redis[0], fill_here_doc(hd_limiter, &writer_pid));
	ft_wait(writer_pid);
	redis[1].kind = OUT_APPEND;
	redi_set_path(&redis[1], outpath);
}

static t_cmd	*pipe_end(t_cmd **pipe)
{
	size_t	i;

	if (*pipe == NULL)
		return (NULL);
	i = 0;
	while (pipe[i + 1])
		i++;
	return (pipe[i]);
}

int	main(int argc, char **argv)
{
	t_redi	redis[2];
	int		result;
	t_cmd	**shell_pipe;

	if (argc <= 4)
		return (print_help(), EXIT_FAILURE);
	if (!ft_strncmp("here_doc", argv[1], 9))
	{
		if (argc <= 5)
			return (print_help(), EXIT_FAILURE);
		build_bonus_redis(&redis[0], argv[2], argv[argc - 1]);
		shell_pipe = (t_cmd **)ft_arr_nmap((t_arr)(&argv[3]), argc - 4,
				my_parse);
	}
	else
	{
		build_normal_redis(&redis[0], argv[1], argv[argc - 1]);
		shell_pipe = (t_cmd **)ft_arr_nmap((t_arr)(&argv[2]), argc - 3,
				my_parse);
	}
	set_redi(shell_pipe[0], &redis[0]);
	set_redi(pipe_end(shell_pipe), &redis[1]);
	result = run_pipe(shell_pipe);
	ft_m3_cleanup();
	return (result);
}
