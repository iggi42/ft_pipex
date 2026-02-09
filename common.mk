# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    common.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkruger <fkruger@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 16:10:11 by fkruger           #+#    #+#              #
#    Updated: 2025/12/11 12:20:33 by fkruger          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# configuration variables
CC = cc
CFLAGS += -MD -Wall -Wextra -Werror $(FT_EXTRA_CFLAGS)

FT_EXTRA_CFLAGS += -DFT_APP_NAME=\"$(NAME)\"

GIT_IGNORE += .depend
GIT_IGNORE += .gdb_history
GIT_IGNORE += $(NAME)

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
DEV_FILES += .gitignore compile_flags.txt
GIT_IGNORE += $(OBJS) $(DEPS) $(DEV_FILES)

LIBFT = ./libft
LIBFT_A = ./libft/libft.a
CFLAGS += -I./libft/inc/
LDLIBS += $(LIBFT_A)

SELF=$(firstword $(MAKEFILE_LIST))

export FT_EXTRA_CFLAGS

# phony targets
all: $(NAME)
re: clean all
fclean: clean dev_clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT) $@
	find -name '*.d' -print -delete
clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT) $@
dev: $(DEV_FILES)
	$(MAKE) -C $(LIBFT) $@
dev_clean:
	$(RM) $(DEV_FILES)
	$(MAKE) -C $(LIBFT) $@
debug: FT_EXTRA_CFLAGS += -g
debug: clean $(NAME)
.PHONY: fclean clean re all dev debug

# development helper files
compile_flags.txt: $(SELF)
	@echo setup $@
	@echo -n > $@
	@for flag in $(CFLAGS); do \
		echo $$flag >> $@ ; \
	done

.gitignore: $(SELF)
	@echo setup $@
	@for ig in $(GIT_IGNORE); do \
		echo $$ig >> $@ ; \
	done

# core build rules
$(LIBFT_A): $(LIBFT)
	$(MAKE) -C $(LIBFT)

GIT_IGNORE += $(DEPS)
-include $(DEPS)
