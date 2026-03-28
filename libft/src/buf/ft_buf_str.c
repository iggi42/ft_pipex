/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buf_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"
#include "libft_buf.h"

char	*ft_buf_str(t_buf **b)
{
	char	*result;
	size_t	i;

	if (*b == NULL || (*b)->size == 0)
	{
		ft_buf_free(b);
		return (NULL);
	}
	result = ft_str_alloc((*b)->size);
	if (result == NULL)
	{
		ft_buf_free(b);
		return (NULL);
	}
	i = 0;
	while (i < (*b)->size)
	{
		*(result + i) = (char)*((*b)->p + i);
		i++;
	}
	*(result + i) = '\0';
	ft_buf_free(b);
	return (result);
}
