# *************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkruger <fkruger@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 16:10:11 by fkruger           #+#    #+#              #
#    Updated: 2025/12/11 12:20:33 by fkruger          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex_cmd
SRCS += pipex_cmd.c mcmd.c pipex_utils.c pipex_parse.c

FT_EXTRA_CFLAGS += -g

-include libft/project.mk

GIT_IGNORE += $(NAME)
$(NAME): $(OBJS) $(LIBFT_A) 
	$(CC) $(CFLAGS) -o $@ $^

GIT_IGNORE += $(NAME)_debug
$(NAME)_debug: $(OBJS) $(LIBFT_A) 
	$(CC) -g $(CFLAGS) -o $@ $^
