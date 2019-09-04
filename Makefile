# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/06 08:51:16 by hgranule          #+#    #+#              #
#    Updated: 2019/09/04 11:30:33 by gdaemoni         ###   ########.fr        #
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
SRC_PATH_SH_VARS = $(SRC_PATH)/sh_vars/
OBJ_PATH = $(WS_PATH)/objects/
INC_PATH = $(WS_PATH)/includes/
LIB_PATH = $(WS_PATH)/Libft/
LIB_INC_PATH = $(LIB_PATH)includes/

SRC = $(addprefix $(SRC_PATH), $(SRC_FILES))
SRC_VAR = $(addprefix $(SRC_PATH_SH_VARS), $(SRC_VAR_FILES))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_FILES))
INC = $(addprefix -I, $(INC_PATH))
INC_LIB = $(addprefix -I, $(LIB_INC_PATH))

SRC_FILES = sh_main.c sh_control_term.c sh_dir_content.c sh_readline.c sh_readline_help.c\
	sh_term.c dstr.c ft_concat.c sh_put_col.c sh_tab.c sh_darr.c sh_path.c reg_expr.c\
	reg_expr_loop.c reg_expr_help.c sh_histrory.c sh_tab_help.c dstr_help.c match.c\
	sh_darr_help.c sh_dir_content_help.c sh_put_col_help.c sh_move_insertion_point.c 
	
SRC_VAR_FILES = sh_vars_gets.c sh_vars_init.c sh_vars_parser.c

OBJ_FILES = $(SRC_VAR_FILES:.c=.o) $(SRC_FILES:.c=.o) 

# ************************************************************************** #
#                                   Rules                                    #
# ************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@make make_lft
	@gcc $(FLAGS) -o $(NAME) $(INC) $(INC_LIB) $(OBJ) -L $(LIB_PATH) -lft -lncurses -g

$(OBJ_PATH)%.o: $(SRC_PATH_SH_VARS)%.c
	@echo "$(GREEN)$(UNDERLINE)CREATING >> $@$(ESCN)"
	@mkdir -p $(OBJ_PATH)
	@gcc $(FLAGS) $(INC) $(INC_LIB) -o $@ -c $<

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@echo "$(RED)$(UNDERLINE)CREATING >> $@$(ESCN)"
	@mkdir -p $(OBJ_PATH)
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
