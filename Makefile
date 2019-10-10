# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgranule <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/06 08:51:16 by hgranule          #+#    #+#              #
#    Updated: 2019/10/10 12:40:03 by hgranule         ###   ########.fr        #
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

SRC_FILES = sh_main.c get_line.c \
			ft_flagsparser.c exe_sys/exe_calls.c free_me/rms.c \
			ft_basename.c exe_sys/exe_binaries.c exe_sys/exe_builtns.c \
			exe_sys/exe_redirs.c parser/prs_arch.c parser/prs_args.c exe_sys/exe_subshell.c \
			parser/prs_instructions.c parser/prs_rdrs.c parser/prs_types.c \
			env/env_init.c env/env_get_variable.c env/env_set_variable.c env/env_unset_variable.c \
			bltns/bltn_init.c bltns/bltn_setenv.c bltns/bltn_env.c bltns/bltn_getenv.c bltns/bltn_unsetenv.c \
			bltns/bltn_exit.c bltns/bltn_alias.c bltns/bltn_unalias.c bltns/bltn_cd.c bltns/bltn_pwd.c\
			bltns/bltn_type/bltn_type.c bltns/bltn_type/bltn_type_free.c bltns/bltn_type/bltn_type_alias_check.c bltns/bltn_type/bltn_type_bins_check.c\
			bltns/bltn_type/bltn_type_help.c\
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
			sys_tools/sys_touch.c $(addprefix gui/, $(GUI_FILES)) \
			sys_tools/sys_config.c sys_tools/sys_files.c sys_tools/sys_proc_tables.c \
			env/env_get_bins/env_get_bins_parsebltn.c env/env_get_bins/env_get_bins_parsefunc.c \
			env/env_get_bins/env_get_bins_parsepath.c env/env_get_bins/env_get_bins.c \
			\
			lexer/lx_backgr_offset.c lexer/lx_bash_init.c \
			lexer/lx_bash_parse.c lexer/lx_cleaners.c \
			lexer/lx_comm_parse.c lexer/lx_envar_parse.c \
			lexer/lx_func_parse.c \
			lexer/lx_hedoc_parse.c lexer/lx_lexer.c \
			lexer/lx_math_init.c \
			lexer/lx_math_parse.c lexer/lx_metatree.c \
			lexer/lx_new_input_finished.c lexer/lx_proc_parse.c \
			lexer/lx_quots_parse.c lexer/lx_redir_init.c \
			lexer/lx_redir_parse.c lexer/lx_regulars.c \
			lexer/lx_sep_parse.c lexer/lx_subsh_parse.c \
			lexer/lx_tokenizer.c lexer/lx_usefull_func.c \
			lexer/lx_validate.c

ADD_OBJ = $(addprefix $(OBJ_PATH), exe_sys free_me parser env bltns aliases bltns/math sys_tools gui env/env_get_bins bltns/bltn_type lexer)

GUI_FILES = sh_control_term.c sh_dir_content.c sh_readline.c sh_readline_help.c\
	sh_term.c dstr.c ft_concat.c sh_put_col.c sh_tab.c sh_darr.c sh_path.c reg_expr.c\
	reg_expr_loop.c reg_expr_help.c sh_histrory.c sh_tab_help.c dstr_help.c match.c\
	sh_darr_help.c sh_dir_content_help.c sh_put_col_help.c sh_move_insertion_point.c\
	sh_search_history.c sh_search_his_help.c gui_errors.c

OBJ_FILES = $(SRC_FILES:.c=.o)

# ************************************************************************** #
#                                   Rules                                    #
# ************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@make make_lft
	@gcc $(FLAGS) -o $(NAME) $(INC) $(INC_LIB) $(OBJ) -L $(LIB_PATH) -lft -lncurses -g

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

ex:
	gcc exmpl.c -lcurses -g -o 42sh -L libft -lft -I libft/includes

appc: $(OBJ)
	@make make_lft
	@gcc $(FLAGS) -o $(NAME) $(INC) $(INC_LIB) $(OBJ) -L $(LIB_PATH) -lft -L /usr/lib/x86_64-linux-gnu/libncurses.so -lncurses

