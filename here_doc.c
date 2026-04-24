/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:24:39 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/23 15:24:40 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redi.h"
#include "utils.h"
#include "bw.h"
#include <errno.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_str.h>
#include <stdlib.h>
#include <unistd.h>

static void	here_doc_write(int fd, char **s)
{
	ft_dup2(fd, STDOUT_FILENO);
	ft_bw_cleanup();
	while (*s)
	{
		if (0 > ft_write(STDOUT_FILENO, *s, ft_strlen(*s)))
			error_out(EXIT_FAILURE, "heredoc write failure", errno);
		s++;
	}
	ft_exit(EXIT_SUCCESS);
}

int	write_in_fork(char **s, pid_t *writer)
{
	int	my_pipe[2];

	ft_pipe(my_pipe);
	*writer = ft_fork();
	if(*writer == 0)
		(ft_close(my_pipe[R]), here_doc_write(my_pipe[W], s));

	ft_close(my_pipe[W]);
	return (my_pipe[R]);
}

// 2 try
char	**read_here_doc2(char *(*gnl)(void), char *delimiter)
{
	t_list	*result;
	char	*line;

	result = NULL;
	line = gnl();
	while (line)
	{
		if (line || !ft_strncmp(delimiter, line, ft_strlen(delimiter) + 1))
			break ;
		ft_lstadd_back(&result, ft_lstnew(line));
		line = gnl();
	}
	// TODO free memory
	return (char **)ft_lst2arr(result);
}

static char *stdin_gnl(void)
{
	return ft_gnl(STDIN_FILENO);
}

int fill_here_doc(char *delimiter, pid_t *writer)
{
	char **lines;

	lines = read_here_doc2(stdin_gnl, delimiter);
	return write_in_fork(lines, writer);
}

/*
// 1 try
char	*read_here_doc(char *(*gnl)(void), char *delimiter)
{
	char	*current_line;
	char	*here_doc;
	size_t	doc_size;
	size_t	req_size;

	current_line = gnl();
	here_doc = ft_strdup("");
	doc_size = 1;
	while (current_line)
	{
		req_size = ft_strlcat(here_doc, current_line, doc_size);
		if (req_size > doc_size)
		{
			ft_str_grow(&here_doc, doc_size * 2);
			continue ;
		}
		current_line = gnl();
		if (!ft_strncmp(delimiter, current_line, ft_strlen(delimiter) + 1))
			break ;
	}
	return (here_doc);
}
*/
