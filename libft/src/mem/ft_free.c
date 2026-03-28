/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 09:19:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:06 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdarg.h>

void	*ft_free(size_t n, ...)
{
	va_list	args;
	void	**target;

	va_start(args, n);
	while (n--)
	{
		target = va_arg(args, void *);
		free(*target);
		*target = NULL;
	}
	va_end(args);
	return (NULL);
}
