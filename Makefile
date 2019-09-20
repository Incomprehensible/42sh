# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgranule <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/06 08:51:16 by hgranule          #+#    #+#              #
#    Updated: 2019/09/20 22:22:29 by hgranule         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, $(NAME), clean, fclean, re, make_lft

# ************************************************************************** #
#                           colors                                           #
# ************************************************************************** #

RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
PINK = \033[35m
AQUA = \033[36m
GREY = \033[37m
UNDERLINE = \033[4m
ESCN = \033[0m

# ************************************************************************** #
#                           Configuration                                    #
# ************************************************************************** #
NAME = 42sh
FLAGS = -g

WS_PATH = $(shell pwd)
SRC_PATH = $(WS_PATH)/sources/
OBJ_PATH = $(WS_PATH)/objects/
INC_PATH = $(WS_PATH)/includes/
LIB_PATH = $(WS_PATH)/Libft/
LIB_INC_PATH = $(LIB_PATH)includes/

SRC = $(addprefix $(SRC_PATH), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_FILES))
INC = $(addprefix -I, $(INC_PATH))
INC_LIB = $(addprefix -I, $(LIB_INC_PATH))

SRC_FILES = sh_main.c exe_sys/exe_calls.c free_me/rms.c \
			ft_basename.c exe_sys/exe_binaries.c exe_sys/exe_builtns.c \
			exe_sys/exe_redirs.c parser/prs_arch.c parser/prs_args.c \
			parser/prs_instructions.c parser/prs_rdrs.c parser/prs_types.c \
			env/env_init.c env/env_get_variable.c env/env_set_variable.c env/env_unset_variable.c \
			bltns/bltn_init.c bltns/bltn_setenv.c bltns/bltn_env.c \
			bltns/bltn_getenv.c bltns/bltn_unsetenv.c \
			bltns/bltn_exit.c bltns/bltn_alias.c bltns/bltn_unalias.c \
			aliases/alias_add.c aliases/alias_init.c \
			exe_sys/exe_funcs.c parser/prs_and_or.c parser/prs_assigm.c \
			parser/prs_funcs.c parser/prs_if_while.c parser/prs_skipers.c \
			parser/prs_error.c parser/prs_core_setter.c \
			\
			bltns/math/bltn_math.c bltns/math/math_ret_var.c bltns/math/math_skipers.c \
			bltns/math/math_ops_dec_inc.c bltns/math/math_ops_log_eq.c \
			bltns/math/math_ops_lw_bg.c bltns/math/math_ops_primary.c \
			\
			sys_tools/sys_init.c sys_tools/sys_pipes.c parser/prs_subsh.c \
			sys_tools/sys_proc_wait.c sys_tools/sys_errors.c sys_tools/sys_io.c \
			sys_tools/sys_touch.c

ADD_OBJ = $(addprefix $(OBJ_PATH), exe_sys free_me parser env bltns aliases bltns/math sys_tools)

OBJ_FILES = $(SRC_FILES:.c=.o)

# ************************************************************************** #
#                                   Rules                                    #
# ************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@make make_lft
	@gcc $(FLAGS) -o $(NAME) $(INC) $(INC_LIB) $(OBJ) -L $(LIB_PATH) -lft -lncurses

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@echo "$(RED)$(UNDERLINE)CREATING >> $@$(ESCN)"
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(ADD_OBJ)
	@gcc $(FLAGS) $(INC) $(INC_LIB) -o $@ -c $<

make_lft:
	@echo "$(RED)$(UNDERLINE)COMPILING LIBFT$(ESCN)"
	@make -C $(LIB_PATH)

clean:
	@rm -rf $(OBJ_PATH)
	@make clean -C $(LIB_PATH)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIB_PATH)

re: fclean all

appc: $(OBJ)
	@make make_lft
	@gcc $(FLAGS) -o $(NAME) $(INC) $(INC_LIB) $(OBJ) -L $(LIB_PATH) -lft -L /usr/lib/x86_64-linux-gnu/libncurses.so -lncurses
