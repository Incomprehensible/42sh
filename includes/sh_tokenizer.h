/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tokenizer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/16 01:28:22 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_42SH_SH_TOKENIZER_H
#define INC_42SH_SH_TOKENIZER_H

#include "sh_req.h"
#include "sh_token.h"

//tree indexes
# define SEPS (short)0x0000000015
# define DQUOTS (short)0x0000000000
# define APOFS (short)0x0000000001
# define MATHS (short)0x0000000002
# define SUBSHS (short)0x0000000003
# define FLOWS (short)0x0000000004
# define ENVAR (short)0x0000000005
# define REDIR (short)0x0000000008
# define FUNCS (short)0x0000000009
# define EXPRS (short)0xB
# define DEREF (short)0xC
# define EMPTY (short)0x0000000013
# define COMMENT (short)0x0000000014
# define PROC (short)0x0000000065
# define PROF (short)0x0000000067
# define CURLY (short)0x0000000068
//TEMPORARY
# define RETURN TK_RETURN

//math tokens for syntax analysis
# define OPERATOR (t_tk_type)0x0000000111
# define OPERAND (t_tk_type)0x0000000222
# define APOF (t_tk_type)0x0000000333
# define APOF_END (t_tk_type)0x0000003333
# define BRACKET (t_tk_type)0x0000000444
# define BRACKET_END (t_tk_type)0x0000004444
# define MATH_SEQ (t_tk_type)0x0000000555

//for redirs
# define CLOSE (t_tk_type)0x0000000016

//for scripts
# define GLUE (short)0x0000000017
# define IN (short)0x0000000018
# define FORMATH (short)0x0000000019
# define MATH_NT (short)0x0000000020
# define EX (short)0x0000000021

int		get_line(int fd, char **line);

typedef struct      s_stx
{
    struct s_stx *next;
    struct s_stx *prev;
    char         *meta;
}                   t_stx;

typedef struct  s_graph
{
    char            *patt;
    t_tk_type		type;
    struct s_graph  *right;
    struct s_graph  *left;
    struct s_graph  *forward;
    struct s_graph  *next;
}               t_graph;

short		sh_tokenizer(char *str, t_dlist **token_list);
t_dlist		**toklst_init(t_dlist **token_list);
char		*pull_token(char *str, size_t i);
t_stx       *init_comm(t_stx *tree);
t_stx       *init_scripts(t_stx *tree);
t_stx       *init_math(t_stx *tree);
t_stx       *init_envar(t_stx *tree);
t_stx       *init_redir(t_stx *tree);
t_stx       *init_subsh(t_stx *tree);
t_stx       *init_func(t_stx *tree);
t_stx       *init_dquotes(t_stx *tree);
t_stx       *init_apofs(t_stx *tree);
t_stx       *init_hedoc(t_stx *tree);
char        *parse_sep(char *str, t_dlist **tok, short i);
char        *parse_dquotes(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_apofs(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_scripts(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_math(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_envar(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_redir(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_func(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_subsh(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_comm(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_deref(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_proc(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_lambda(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_exec(char *s, t_dlist **tok);
char        *parse_empty(char *str, char *patt, t_dlist **tok);
void        make_token(t_dlist **list, char *value, t_tk_type type);
short		clear_tokens(t_dlist **tokens, short id);
int         special_meta(char meta);
char        *ft_process_s(char *str, char *meta);
char        *ft_process_space(char *str, char *meta);
char        *ft_process_seq(char *str, char *meta);
char        *ft_process_any(char *str, char *meta);
char        *process_reg(char *str, char *meta);
char        *get_point(char *meta);
char        *block_pass(short i, char *str, t_dlist **tok, t_stx **tree);
short       find_token(t_stx **tree, char *str);
int         check_branch(char *str, t_stx *tree);
char        *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr);
short       is_token_here(char *start, char *meta);
short       input_finished(char *str);
void        tree_init(t_stx **tree);
t_graph     *if_script_in(void);
t_graph     *until_script_in(void);
t_graph     *for_script_in(void);
t_graph     *while_script_in(void);
t_graph     *redir_in(void);
t_graph     *redir_two(void);
t_graph     *redir_one(void);
t_graph     *let_math_in(void);
t_graph     *brackets_math_in(void);
char		*check_subbranch(char *str, t_dlist **tok, t_stx **tree);
short       graph_forward_only(t_graph *g);
short       graph_end(t_graph *g, char *str);
char        *pull_expr2(char *str, t_stx **tr, t_dlist **tok);
char        *pull_expr1(char *patt, char *str, t_stx **tr, t_dlist **tok);
char		*pull_subsh(char *str, t_dlist **tok, t_tk_type type);
int         layer_parse_one(char *meta, char *str);
size_t      layer_parse_two(char *meta, char *str);
char        *pull_token(char *str, size_t i);
char        *script_pull(char *patt, t_tk_type type, char *str, t_stx **tr, t_dlist **tok);
char        *reg_process(char *patt, t_tk_type type, char *str, t_dlist **tok, t_stx **tr);
char        *parse_str_block(char *str, t_dlist **tok, t_stx **tree, short br);
short       is_separator(char str);
short		unexpected_token(t_dlist **tok);
short       is_sep_no_space(char str);
char        *ft_process_wall(char *str, char *meta);
char        *ft_process_ignore(char *str, char *meta);
char        *skip_spaces(char *str);
size_t      valid_math_seq(char *str);
size_t      valid_operand(char *str, size_t i);
size_t      valid_operator(char *str, size_t i);
short       valid_apof(char *str, size_t i);
size_t      valid_bracket(char *str, char *patt);
short       valid_sep(char *str);
short		closed_dels(char *str);
t_tk_type   make_offset(t_dlist **token, t_tk_type type);
size_t      get_seq(char *str);
short       list_ready_to_go(t_dlist **token_list);
short       is_redir(char *str);
size_t      remove_spaces(char *str, size_t len);
short       is_q(char c);
short		sep_detected(t_dlist *token_list);
size_t      can_pull_tk(size_t j, char *str, t_dlist **tok, short t);
short       special_case(char br, char *str);
short       check_valid_sep(t_dlist *token_list);
short       back_ps_check(t_dlist *token_list);
short       seps_check(t_dlist *token_list);
short       is_prefix(char str);
short       is_tok_redir(t_tk_type type, short id);
char        get_code(void);
char		*into_envar(char *str, t_dlist **tok, t_stx **tree);
size_t		skip_field(char *str, char skip);
char		*skip_comment(char *str);
char		*markup_station(char *str, t_tk_type type);
void		*ft_arrmemdel(void **ap);
short		valid_deref(char *str);

//more comfortable form of type cast
# define TOK_TYPE ((t_tok *)(token_list->content))->type
# define TOK_VALUE ((t_tok *)(token_list->content))->value

//MISTAKES
short   PARSE_ERR;
short   SYNTAX_ERR;
short   INPUT_NOT_OVER;

//for INPUT_NOT_OVER and SYNTAX_ERR
# define PRO_SUBSH  0
# define PRO_PIPE   1
# define PRO_AND    2
# define PRO_OR     3
# define PRO_NONE   4
# define PRO_APOF   5
# define PRO_DQUOT  6
# define PRO_SQU    7
# define PRO_LAM    8

//for SYNTAX_ERR
# define PRO_IF     11
# define PRO_FOR    12
# define PRO_WHILE  13
# define PRO_DO     14
# define PRO_THEN   15
# define PRO_ELSE   16

//for MATH
typedef struct      s_mtx
{
    struct s_mtx	*next;
    t_tk_type		base;
    char			*strt_meta;
	char			*fin_meta;
}                   t_mtx;

#define INCRM (t_tk_type)1 //++
#define DECRM (t_tk_type)3 //--
#define NOT (t_tk_type)4 //~
#define NEGAT (t_tk_type)5 //-a
#define POSIT (t_tk_type)6 //+a
#define REJECT (t_tk_type)8 //!

#define BIT_R (t_tk_type)9 //>>
#define BIT_L (t_tk_type)11 //<<

#define AND (t_tk_type)12 //&
#define OR (t_tk_type)13 //|
#define XOR (t_tk_type)14 //^

#define MULT (t_tk_type)15 //*
#define DEVID (t_tk_type)17 // /
#define DEVREM (t_tk_type)18 //%

#define PLUS (t_tk_type)19 //+
#define MINUS (t_tk_type)20 //-

#define EQU (t_tk_type)21 //=
#define PLUS_EQ (t_tk_type)22 //+=
#define MIN_EQ (t_tk_type)23 //-=
#define IS_EQU (t_tk_type)24 //==
#define NO_EQU (t_tk_type)25 //!=
#define MORE_EQ (t_tk_type)26 //>=
#define LESS_EQ (t_tk_type)27 //<=
#define MORE (t_tk_type)28 //>
#define LESS (t_tk_type)29 //<

#define LOG_AND (t_tk_type)30 // &&
#define LOG_OR (t_tk_type)31 // ||

#define INTO_BR (t_tk_type)32 // (
#define OUT_BR (t_tk_type)33 // )

#define BIN (t_tk_type)2
#define SEV (t_tk_type)7
#define DEC (t_tk_type)10
#define HEX (t_tk_type)16
#define OPRND (t_tk_type)34 

//MATH ERRORS
#define ERR t_err

typedef struct      s_err
{
    int				err_code;
    char			*error_msg;
}                   t_err;

#define VALUE_TOO_GREAT (int)666
#define INVALID_OP (int)228
#define DOUBLE_NEGATION (int)1488 //double or multiple negation 
#define INVALID_INFIX (int)2007
#define DOUBLE_COMPARE (int)1337
#define OPERAND_EXP (int)2012 //syntax error: operand expected
#define INVALID_ASSIG (int)272 //attempted assignment to non-variable
#define STR_OPERAND (int)47
#define DIVISION_ZERO (int)282
#define WEIRD_ERR (int)401

//TMP
long		ariphmetic_eval(char *expr, ENV *env, ERR *err);
void		*set_error(char *err_token, int code, ERR *err);
void		init_num_bases(t_mtx **bases);
long		ariphmetic_calc(t_dlist **dimon_loh, ENV *env, ERR *err);
void		ops_init(t_tk_type **ops);
void		del_tokens(t_dlist *token);
t_dlist		*lst_to_end(t_dlist *stack);
t_dlist		*push_to_stack(t_dlist *stack, t_dlist *new_elem);
long		polish_calculate(t_dlist **polish_not, ENV *env, ERR *err);
long		apply_to_single(long var, t_tk_type op);
long		logic_ops(long a, long b, t_tk_type op, ERR *err);
long		compare_ops(long a, long b, t_tk_type op, ERR *err);
long		bit_ops(long a, long b, t_tk_type op, ERR *err);
long		normal_ops(long a, long b, t_tk_type op, ERR *err);
long		assign_ops(long a, long b, t_tk_type op, ERR *err);

//DEBUGGING
void			DBG_PRINT_TOKENS(t_dlist *toklst);
void			DBG_PRINT_MATH(t_dlist *toklst);

#endif