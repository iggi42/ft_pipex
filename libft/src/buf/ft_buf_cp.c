/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_cp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft_buf.h"

void	ft_buf_cp(t_buf *from, t_buf *to)
{
	size_t	i;

	if (from->size > to->size)
		return ;
	to->size = from->size;
	i = 0;
	while (i < to->size)
	{
		*(to->p + i) = *(from->p + i);
		i++;
	}
}
