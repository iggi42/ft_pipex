/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft_buf.h"
#include "libft_mem.h"

t_buf	*ft_buf_new(size_t size)
{
	t_buf	*result;

	result = (t_buf *) ft_malloc(sizeof(t_buf));
	result->size = size;
	result->p = (t_byte *) ft_malloc(size * sizeof(t_byte));
	return (result);
}
