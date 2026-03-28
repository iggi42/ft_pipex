/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft_buf.h"
#include "libft_mem.h"
#include "libft_io_opts.h"
#include <unistd.h>

t_buf	*ft_buf_read(int fd)
{
	t_byte	*p;
	int		read_result;
	t_buf	*result;

	p = (t_byte *)malloc(BUFFER_SIZE);
	if (p == NULL)
		return (NULL);
	read_result = read(fd, p, BUFFER_SIZE);
	if (read_result <= 0)
		return (ft_free(1, &p));
	result = (t_buf *)malloc(sizeof(t_buf));
	if (result == NULL)
		return (ft_free(1, &p));
	result->size = read_result;
	result->p = p;
	return (result);
}
