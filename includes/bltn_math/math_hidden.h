/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_hidden.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:13:30 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/03 01:11:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_HIDDEN_H
# define MATH_HIDDEN_H

# include "parser.h"
# include "bltn.h"
# include "ft_ctype.h"

typedef struct		s_mtx
{
	struct s_mtx	*next;
	t_tk_type		base;
	char			*strt_meta;
	char			*fin_meta;
}					t_mtx;

/*
** ++
*/
# define INCRM (t_tk_type)1

/*
** --
*/
# define DECRM (t_tk_type)3

/*
** ~
*/
# define NOT (t_tk_type)4

/*
** -a
*/
# define NEGAT (t_tk_type)5

/*
** +a
*/
# define POSIT (t_tk_type)6

/*
** !
*/
# define REJECT (t_tk_type)8

/*
** >>
*/
# define BIT_R (t_tk_type)9

/*
** <<
*/
# define BIT_L (t_tk_type)11

/*
** &
*/
# define AND (t_tk_type)12

/*
** |
*/
# define OR (t_tk_type)13

/*
** ^
*/
# define XOR (t_tk_type)14

/*
** *
*/
# define MULT (t_tk_type)15

/*
** /
*/
# define DEVID (t_tk_type)17

/*
** %
*/
# define DEVREM (t_tk_type)18

/*
** +
*/
# define PLUS (t_tk_type)19

/*
** -
*/
# define MINUS (t_tk_type)20

/*
** =
*/
# define EQU (t_tk_type)21

/*
** +=
*/
# define PLUS_EQ (t_tk_type)22

/*
** -=
*/
# define MIN_EQ (t_tk_type)23

/*
** ==
*/
# define IS_EQU (t_tk_type)24

/*
** !=
*/
# define NO_EQU (t_tk_type)25

/*
** >=
*/
# define MORE_EQ (t_tk_type)26

/*
** <=
*/
# define LESS_EQ (t_tk_type)27

/*
** >
*/
# define MORE (t_tk_type)28

/*
** <
*/
# define LESS (t_tk_type)29

/*
** &&
*/
# define LOG_AND (t_tk_type)30

/*
** ||
*/
# define LOG_OR (t_tk_type)31

/*
** (
*/
# define INTO_BR (t_tk_type)32

/*
** )
*/
# define OUT_BR (t_tk_type)33

# define BIN (t_tk_type)2
# define SEV (t_tk_type)7
# define DEC (t_tk_type)10
# define HEX (t_tk_type)16
# define OPRND (t_tk_type)34

/*
** MATH ERRORS
*/
# define VALUE_TOO_GREAT (int)666
# define INVALID_OP (int)228
# define DOUBLE_COMPARE (int)1337
# define OPERAND_EXP (int)2012
# define INVALID_ASSIG (int)1488
# define STR_OPERAND (int)47
# define DIVISION_ZERO (int)282
# define WEIRD_ERR (int)2007

long				ariphmetic_eval(char *expr, ENV *env, ERR *err);
void				init_num_bases(t_mtx **bases);
void				first_level_in(t_tk_type *arr);
void				second_level_in(t_tk_type *arr);
void				third_level_in(t_tk_type *arr);
void				assign_level_in(t_tk_type *arr);
void				logic_level_in(t_tk_type *arr);
void				bit_level_in(t_tk_type *arr);
void				normal_level_in(t_tk_type *arr);
void				compare_level_in(t_tk_type *arr);
long				ariphmetic_calc(t_dlist **dimon_loh, ENV *env, ERR *err);
short				is_op(char op);
short				is_operand_tok(t_tk_type type);
short				is_bracket(t_tk_type type);
t_tk_type			is_number(char *value);
short				stop_token(t_tk_type stop, t_tk_type current);
t_dlist				*go_through_brackets(t_dlist *dimon_loh, t_tk_type type);
short				get_operator_tok(t_tk_type *ops, t_tk_type type);
short				get_op_type(t_tk_type op);
t_tk_type			get_base(char *op, t_mtx **bases, t_dlist **math);
size_t				get_base_seq(char *str, char *meta);
char				*get_operand(char *expr);
long				fetch_operand(t_tok *operand, ENV *env, ERR *err);
size_t				parse_base(char *meta, char *str);
char				*pull_bracket(char *expr, t_dlist **math);
char				*pull_bit_offset(char *expr, t_dlist **math, ERR *err);
char				*pull_predessor(char *expr, t_dlist **math, ERR *err);
char				*pull_math_compr(char *expr, t_dlist **math, ERR *err);
char				*pull_operand(char *expr, t_dlist **math, ERR *err);
char				*pull_operator(char *expr, t_dlist **math);
char				*pull_number(char *expr, t_dlist **math, ERR *err);
size_t				parse_base(char *meta, char *str);
void				ops_init(t_tk_type *ops);
void				del_tokens(t_dlist *token);
t_dlist				*lst_to_end(t_dlist *stack);
t_dlist				*push_to_stack(t_dlist *stack, t_dlist *new_elem);
short				pop_operator(t_dlist *op_stack, t_tk_type new_tok);
char				*env_set(char *new, long res, ENV *env);
char				*set_var(t_dlist *opd_stack, ENV *env, ERR *err);
t_dlist				*set_new_var(t_dlist *opd_stack, ENV *env, ERR *err);
long				polish_calculate(t_dlist **polish_not, ENV *env, ERR *err);
t_dlist				*equate_opd(t_dlist *opd_stack, ENV *env, ERR *err);
long				apply_to_single(long var, t_tk_type op);
long				logic_ops(long a, long b, t_tk_type op, ERR *err);
long				compare_ops(long a, long b, t_tk_type op, ERR *err);
long				bit_ops(long a, long b, t_tk_type op, ERR *err);
long				normal_ops(long a, long b, t_tk_type op, ERR *err);
long				assign_ops(long a, long b, t_tk_type op, ERR *err);
long				pull_from_base(char *value, t_tk_type type);
t_dlist				*substitute_single(t_dlist *opd_stack, ENV *env, \
long res, t_tk_type op);
t_dlist				*substitute_both(t_dlist *opd_stack, ENV *env, \
long res, t_tk_type op);
t_dlist				*clean_op_stack(t_dlist *ops);
size_t				count_dlist(t_dlist *list);
t_dlist				*prepare_op_stack(t_dlist *ops, t_tk_type op);
void				*set_error(char *err_token, int code, ERR *err);
void				*error_process(char *new, DSTRING *val, int code, ERR *err);
t_dlist				*process_opd_err(t_dlist *opd_stack, ERR *err);
long				check_result(t_dlist *opd_stack, ENV *env, ERR *err);

/*
** DEBUG
*/
void				dbg_print_math(t_dlist *toklst);

long				math_eval(DSTRING *expr, ENV *envr);

size_t				op_search(DSTRING *expr, char *op, size_t op_len);
int					m_skip_brackets(char *str, size_t *ind);
void				skip_word(char *str, size_t *ind);
void				skip_alnums_n_space(char *str, size_t *ind);
void				m_skip_spaces(char *str, size_t *ind);

DSTRING				*get_res_var(long res);
char				*get_name_var(char *str_xp);
long				expr_ret(DSTRING *expr, ENV *envr);
long				value_ret(char *str, ENV *envr);

long				dec_inc_do(DSTRING *ex, ENV *envr, int is_dec);
long				mop_inc(DSTRING *ex, ENV *envr);
long				mop_dec(DSTRING *ex, ENV *envr);

long				mop_neq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_eq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_and(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_or(DSTRING *ex1, DSTRING *ex2, ENV *envr);

long				mop_lw(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_bg(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_lweq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_bgeq(DSTRING *ex1, DSTRING *ex2, ENV *envr);

long				mop_mod(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_suj(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_div(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_mlt(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long				mop_add(DSTRING *ex1, DSTRING *ex2, ENV *envr);

#endif
