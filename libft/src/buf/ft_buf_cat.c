/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_cat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft_buf.h"

t_buf	*ft_buf_cat(t_buf **left, t_buf **right)
{
	size_t	i;
	t_buf	*result;
	t_byte	*src;

	i = 0;
	result = ft_buf_new((*left)->size + (*right)->size);
	if (result == NULL || result->p == NULL)
		return (NULL);
	while (i < result->size)
	{
		if (i < (*left)->size)
			src = (*left)->p + i;
		else
			src = (*right)->p + (i - (*left)->size);
		*(result->p + i++) = *src;
	}
	return (result);
}
