/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_mem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:32:03 by fkruger           #+#    #+#             */
/*   Updated: 2026/02/13 18:39:04 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MEM_H
# define LIBFT_MEM_H
# include <stdbool.h>
# include <stdlib.h>

/// a single byte for bytewise operations
typedef unsigned char	t_byte;

//! @name memory
//!@{

/**  @brief set n bytes starting from s with c as character */
void					*ft_memset(void *s, int c, size_t n);

/**  @brief set n bytes starting from s with 0 */
void					ft_bzero(void *s, size_t n);

/** @brief checks if memory area is zero for n bytes */
bool					ft_iszero(const void *s, size_t n);

/**  @brief copy n bytes from src to dest,
 * undefined behaviour with overlapping pointers. */
void					*ft_memcpy(void *dest, const void *src, size_t n);

/**  @brief move n bytes from src to dest, handles overlapping pointers. */
void					*ft_memmove(void *dest, const void *src, size_t n);

/**  @brief find pointer with value of c within n bytes starting from s. */
void					*ft_memchr(const void *s, const t_byte c, size_t n);

/**  @brief compare s1 and s2 with in n bytes. */
int						ft_memcmp(const void *s1, const void *s2, size_t n);

/**  @brief wraps a malloc in common error handling (exit out on failure) */
void					*ft_malloc(size_t size);

/**  @brief alloc a zeroed out memory for an array of n elements
 * with a size of `size`. */
void					*ft_calloc(size_t n, size_t size);

/** @brief free and zero all variables pointed arguments,
 * always returns NULL.
 **/
void					*ft_free(size_t n, ...);

void					ft_switch(int *a, int *b);

//!@}
#endif
