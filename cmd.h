/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 08:47:52 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 08:47:54 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "redi.h"
# include <libft_ll.h>
# include <libft_mem.h>

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_NO_EXEC_PERM 126

// reds are redirections see struct above for what the ll carry
typedef struct s_cmd
{
	char		**argv;
	t_redi		**reds;
}				t_cmd;

typedef t_cmd	*t_pipe;

t_byte			pipex_exec_pipe(t_pipe *full_pipe);
size_t			parse_cmd(char *textform, t_cmd *target);
#endif
