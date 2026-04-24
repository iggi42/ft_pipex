/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 17:41:22 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 17:41:24 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDI_H
# define REDI_H
# include "here_doc.h"

enum						e_pipe_end
{
	R = 0,
	W = 1
};

typedef struct s_redi
{
	enum					e_redi_kind
	{
		INVALID = -1,
		IN,
		OUT,
		OUT_APPEND,
		HERE_DOC
	} kind;
	enum					e_redi_source
	{
		FD,
		PATH
	} source_kind;
	union					u_redi_source
	{
		char				*path;
		int					fd;
	} source;
}							t_redi;

typedef enum e_redi_kind	t_red_k;

void						redi_set_path(t_redi *r, char *path);
void						redi_set_fd(t_redi *r, int fd);

void						apply_redi(t_redi *apply_me);

#endif
