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

NAME = pipex
HEADER = $(NAME).h
SRCS += pipex_main.c pipex_data.c
FT_EXTRA_CFLAGS += -g -Xanalyzer

-include libft/project.mk

GIT_IGNORE += 1 2 3 4 5 6 7 8 9

GIT_IGNORE += $(NAME)
$(NAME): $(OBJS) $(LIBFT_A) 
	$(CC) $(CFLAGS) -o $@ $^

GIT_IGNORE += $(NAME)_debug
$(NAME)_debug: $(OBJS) $(LIBFT_A) 
	$(CC) -g $(CFLAGS) -o $@ $^
