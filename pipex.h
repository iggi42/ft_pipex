/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:32:03 by fkruger           #+#    #+#             */
/*   Updated: 2025/12/05 16:51:43 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>

void	error_out(int exit_code, char *msg, int error_code);
pid_t	start_first_cmd(char *cmd, int *pipe_fds, char *infile_path);
pid_t	start_last_cmd(char *cmd, int *pipe_fds, char *outfile_path);
int		open_infile(char *infile_path);
int		open_outfile(char *outfile_path);
void	close_pipe(int *pipe);
void	mv_fd(int from, int to);
pid_t	ft_fork(void);

#endif
