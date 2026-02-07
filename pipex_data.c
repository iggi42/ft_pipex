/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:52:58 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/07 04:53:15 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pipex* pipex_data(void)
{
	static t_pipex core;

	return &core;
}

static void check_fd(int *fd)
{
	if (*fd < 0)
		return ;
	close(*fd);
	*fd = -1;
}

void pipex_cleanup(void)
{
	t_pipex *data = pipex_data();
	
	check_fd(&data->fds[0]);
	check_fd(&data->fds[1]);
	check_fd(&data->fds[2]);
	check_fd(&data->fds[3]);
}
