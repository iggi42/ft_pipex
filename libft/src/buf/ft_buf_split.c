/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft_buf.h"

t_buf	*ft_buf_split(t_buf **b, size_t left_len)
{
	t_buf	*left;
	t_buf	*right;
	t_byte	*target;
	size_t	i;

	if (left_len >= (*b)->size)
		return (ft_buf_new(0));
	left = ft_buf_new(left_len);
	right = ft_buf_new((*b)->size - left_len);
	if (left->p == NULL || right->p == NULL)
		return (NULL);
	i = 0;
	while (i < (*b)->size)
	{
		if (i < left_len)
			target = left->p + i;
		else
			target = right->p + (i - left_len);
		*target = *((*b)->p + i++);
	}
	*b = left;
	return (right);
}
