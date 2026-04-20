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
#include "redi.h"
#include "utils.h"
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_merle.h>
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

int	main(int argc, char **argv)
{
	t_redi	redis[2];
	int		result;
	t_cmd	**shell_pipe;

	if (argc <= 4)
		return (print_help(), EXIT_FAILURE);
	redis[0].kind = IN;
	redis[0].target = argv[1];
	redis[1].kind = OUT;
	redis[1].target = argv[argc - 1];
	shell_pipe = (t_cmd **)ft_arr_nmap((t_arr)(&argv[2]), argc - 3, my_parse);
	set_redi(shell_pipe[0], &redis[0]);
	set_redi(shell_pipe[argc - 4], &redis[1]);
	result = pipex_exec_pipe(shell_pipe);
	ft_m3_cleanup();
	return (result);
}
