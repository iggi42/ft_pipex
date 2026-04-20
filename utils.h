/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:08:24 by fkruger           #+#    #+#             */
/*   Updated: 2026/04/19 09:08:43 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <stdlib.h>

void	ft_exit(int exit_code);

void	*my_malloc(size_t size);
void	print_str_arr(char **ar);

void	error_out(int exit_code, char *msg, int error_code);

// in bw_utils.c
pid_t	ft_fork(void);
void	ft_close(int fd);
void	ft_dup2(int from, int to);
// void	mv_fd(int from, int to);
int		*ft_pipe(int *new_pipe);

#endif
