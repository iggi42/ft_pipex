/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 17:44:26 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 17:44:27 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "bw.h"
#include "redi.h"
#include "stdio.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <libft_io.h>
#include <libft_str.h>
#include <stdlib.h>
#include <unistd.h>

int	open_infile(char *infile_path)
{
	int	fd;

	errno = 0;
	fd = open(infile_path, O_RDONLY | O_CLOEXEC);
	if (fd >= 0)
		return (fd);
	error_out(EXIT_FAILURE, infile_path, errno);
	return (-1);
}

int	open_outfile(char *outfile_path)
{
	int	fd;

	errno = 0;
	fd = open(outfile_path, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0644);
	if (fd >= 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}

int	open_outappfile(char *outfile_path)
{
	int	fd;

	errno = 0;
	fd = open(outfile_path, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0644);
	if (fd >= 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}

int	tmp_fd(void)
{
	int	fd;

	fd = open("/tmp", O_TMPFILE | O_RDWR | O_CLOEXEC, 0600);
	if (fd >= 0)
		return (fd);
	error_out(EXIT_FAILURE, "tmpfile", errno);
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
	error_out(EXIT_FAILURE, "unexpected redirector kind", 0);
	return (NULL);
}

#define ERR_MSG_REDI_SRC "unexpected redirection source"

void redi_set_path(t_redi *r, char *path)
{
	if(r == NULL)
		return;
	r->source_kind = PATH;
	r->source.path = path;
}

void redi_set_fd(t_redi *r, int fd)
{
	if(r == NULL)
		return;
	r->source_kind = FD;
	r->source.fd = fd;
}

void	apply_redi(t_redi *apply_me)
{
	int	fd;
	int	target;

	if (apply_me == NULL)
		return ;
	errno = 0;
	if (apply_me->source_kind == FD)
		fd = apply_me->source.fd;
	else if (apply_me->source_kind == PATH)
		fd = get_opn(apply_me->kind)(apply_me->source.path);
	else
		fd = (error_out(EXIT_FAILURE, ERR_MSG_REDI_SRC, errno), -1);
	ft_bw_add(fd);
	if (apply_me->kind == OUT || apply_me->kind == OUT_APPEND)
		target = STDOUT_FILENO;
	else
		target = STDIN_FILENO;
	ft_dup2(fd, target);
}
