/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_out.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:40:11 by fkruger           #+#    #+#             */
/*   Updated: 2026/03/27 14:40:12 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <libft_io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	error_out(int exit_code, char *msg, int error_code, bool free_msg)
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
	if (free_msg)
		free(msg);
	exit(exit_code);
}

void	*check(void *check_me, char *err_msg, bool free_msg)
{
	if (errno == 0 && check_me != NULL)
		return (check_me);
	error_out(EXIT_SUCCESS, err_msg, errno, free_msg);
	return (NULL);
}
