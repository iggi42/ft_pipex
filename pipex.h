/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:32:03 by fkruger           #+#    #+#             */
/*   Updated: 2025/12/05 16:51:43 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft_ll.h>
# include <unistd.h>

// fds:
// 0 => input file && cmd1 stdin
// 1 => cmd1 stdout
// 2 => cmd2 stdin
// 3 => cmd2 stdout && output file
typedef struct s_pipex
{
	int		fds[4];
	pid_t	cmds[2];
}			t_pipex;

t_pipex *pipex_data(void);

void pipex_cleanup(void);

#endif
