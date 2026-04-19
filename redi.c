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


#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"
#include <libft_io.h>
#include "bw.h"
#include "redi.h"

int	open_infile(char *infile_path)
{
	int	fd;

	errno = 0;
	fd = open(infile_path, O_RDONLY);
	if (fd >= 0)
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
	if (fd >= 0)
		return (fd);
	error_out(EXIT_FAILURE, outfile_path, errno);
	return (-1);
}

int	open_heredoc(char *end)
{
	(void)end;
	ft_putendl_fd("HEREDOC IS TODO", STDERR_FILENO);
	ft_exit(42);
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

void	apply_redi(t_redi *apply_me)
{
	int		fd;
	int target;

	if (apply_me == NULL)
		return ;
	fd = (get_opn(apply_me->kind))(apply_me->target);
	ft_bw_add(fd);

	if(apply_me->kind == OUT)
		target = STDOUT_FILENO;
	else
		target = STDIN_FILENO;
	mv_fd(fd, target);
}
