/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tokenizer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 03:28:58 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_42SH_SH_TOKENIZER_H
# define INC_42SH_SH_TOKENIZER_H

# include "sh_req.h"
# include "sh_token.h"
# include "sys_tools/sys_errors.h"
# include "bltn_math/math_hidden.h"

# define SEPS (short)0x0000000015
# define DQUOTS (short)0x0000000000
# define APOFS (short)0x0000000001
# define MATHS (short)0x0000000002
# define SUBSHS (short)0x0000000003
# define FLOWS (short)0x0000000004
# define ENVAR (short)0x0000000005
# define HERED (short)0x0000000006
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

typedef struct	  s_stx
{
	struct s_stx *next;
	struct s_stx *prev;
	char		 *meta;
}				  t_stx;

typedef struct  s_graph
{
	char			*patt;
	t_tk_type		type;
	struct s_graph  *right;
	struct s_graph  *left;
	struct s_graph  *forward;
	struct s_graph  *next;
}			   t_graph;

short		sh_tokenizer(char *str, t_dlist **token_list);
t_dlist		**toklst_init(t_dlist **token_list);
char		*null_last_input(char **last_input);
short       scripts_closed(char *str);
char        *cycle_to_the_start(char *str);
char        *if_to_the_start(char *str);
short       quotes_closed(char *str);
char		*qa_qt_cls_n(char *str, int *q);
short       q_closed(char *str, char q, char q1, char q2);
short		input_closed(char *str);
short		brackets_closed(char *str);
short       br_closed(char *s, char strt, char fin);
char        *br_cls_n(char *str, short *i);
char        *br_qt_cls_n(char *str, short *i);
void        tree_init(t_stx **tree);
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
char		*parse_sep(char *str, t_dlist **tok, short i);
char		*parse_dquotes(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_apofs(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_scripts(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_math(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_envar(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_redir(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_func(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_subsh(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_comm(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_deref(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_proc(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_lambda(char *str, t_dlist **tok, t_stx **tree, short i);
char		*parse_exec(char *s, t_dlist **tok);
char		*parse_empty(char *str, char *patt, t_dlist **tok);
char        *parse_assig_block(char *str, t_dlist **tok, t_stx **tree);
char		*parse_str_block(char *str, t_dlist **tok, t_stx **tree, int br);
void		make_token(t_dlist **list, char *value, t_tk_type type);
short       find_token(t_stx **tree, char *str);
short		clear_tokens(t_dlist **tokens, short id);
char		*ft_process_s(char *str, char *meta);
char		*ft_process_space(char *str, char *meta);
char		*ft_process_seq(char *str, char *meta);
char		*ft_process_any(char *str, char *meta);
char        *ft_process_limits(char *str);
char        *ft_process_ignore(char *str, char *meta);
char        *ft_process_all(char *str, char *meta);
char		*ft_process_wall(char *str, char *meta);
char		*ft_process_trash(char *str, char *meta);
char		*process_reg(char *str, char *meta);
char		*block_pass(short i, char *str, t_dlist **tok, t_stx **tree);
int         check_branch(char *str, t_stx *tree);
char		*check_subbranch(char *str, t_dlist **tok, t_stx **tree);
char		*script_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok);
char		*into_the_portal(t_graph *g, char *s, t_dlist **tok, t_stx **tr);
short       input_finished(char *str);
t_graph     *if_script_in(void);
t_graph     *until_script_in(void);
t_graph     *for_script_in(void);
t_graph     *while_script_in(void);
char		*expr_in(char *s, t_graph *g, t_stx **tr, t_dlist **tk);
t_graph     *do_init(t_graph *tk);
t_graph     *done_init(void);
t_graph     *forexpr_init(t_graph *fortk, t_tk_type tk);
t_graph     *condition_init(t_graph *whiletk);
t_graph     *sep_init(void);
t_graph     *exprtk2_init(void);
t_graph     *fd_in(void);
t_graph     *prof_in(void);
t_graph     *redir_in(void);
t_graph     *redir_two(void);
t_graph     *redir_one(void);
short       redir_proceeded(t_dlist *token_list);
char        *redirect_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok);
char        *redir_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok);
char        *fd_pull(t_graph *g, char *s, t_dlist **tok);
short       graph_forward_only(t_graph *g);
short       graph_end(t_graph *g, char *str);
short       graph_type(char *str);
char		*pull_token(char *str, size_t i);
char		*pull_expr2(char *str, t_stx **tr, t_dlist **tok);
char		*pull_expr1(char *patt, char *str, t_stx **tr, t_dlist **tok);
char		*pull_subsh(char *str, t_dlist **tok, t_tk_type type);
char		*pull_token(char *str, size_t i);
char		*pull_single(char *str);
char		*pull_break_cont(char *s, t_tk_type tp, t_dlist **tok);
char        *pull_legit_math(char *s, t_dlist **tok, t_stx **tr);
char		*script_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok);
int         layer_parse_one(char *meta, char *str);
size_t      layer_parse_two(char *meta, char *str);
char		*reg_process(char *patt, t_tk_type type, char *str, t_dlist **tok);
size_t      decimals_proceed(char *str, char *meta);
short		closed_dels(char *str);
t_tk_type   make_offset(t_dlist **token, t_tk_type type);
short       list_ready_to_go(t_dlist **token_list);
short       is_separator(char str);
short       is_sep_no_space(char str);
short       is_meta(char *str, char meta);
short       is_q(char c);
short		is_assign(char c);
short		is_it_q(char c);
short       is_it_br(char c);
short       is_prefix(char str);
short       is_ariphmetic(char str);
short       is_redir(char *str);
short       is_tok_redir(t_tk_type type, short id);
short       is_sep_token(t_tk_type type);
short		is_flow(t_tk_type type);
short       is_token_here(char *start, char *meta);
short		are_tokens_here(char *str);
short		sep_detected(t_dlist *token_list);
short       seps_check(t_dlist *token_list);
short       back_ps_check(t_dlist *token_list);
short       check_valid_sep(t_dlist *token_list);
short       args_check(t_dlist *token_list);
size_t      can_pull_tk(size_t j, char *str, t_dlist **tok, int t);
int         special_meta(char meta);
short       special_case(char br, char *str);
short		special_condition(char *patt);
size_t      get_seq(char *str, char *meta);
char		get_code(void);
char		*get_point(char *meta);
char		*into_envar(char *str, t_dlist **tok, t_stx **tree);
size_t      remove_spaces(char *str, size_t len);
void		*ft_arrmemdel(void **ap);
short		unexpected_token(t_dlist **tok);
short		got_in_seq(char sym, char *seq);
t_dlist		*skip_function(t_dlist *token_list);
t_dlist		*skip_done_script(t_dlist *token_list);
t_dlist		*skip_if_script(t_dlist *token_list);
char        *skip_brackets(char *str, char br);
char		*skip_spaces(char *str);
size_t		skip_field(char *str, char skip);
char        *skip_spaces_nd_nl(char *str);
char		*skip_comment(char *str);
char		*skip_spaces_newline(char *str);
size_t		mirror_passes(char *str);
short		mirrored(char *arr);
char		*assig_into_portal(char *str, t_dlist **tok, t_stx **tree);
void		exec_on(t_dlist *token_list);
short       following_pipe(t_dlist *token_list);
short		valid_deref(char *str);
short       valid_sep(char *str);
size_t      validate_simple_struct(char *s, size_t br);
size_t      validate_triple_struct(char *s, short pass);
short       validate_var(char *varname);
short       validate_envar(char *str);
int			validate_ifs(char *str);
void		expr_to_value(t_dlist *token_list);
void		substitute_value(t_dlist *token_list);
char        *cook_err_msg(char *insertion);
char		*markup_station(char *str, t_tk_type type);
char		*cutting_mirr_station(char *str, t_tk_type type);
size_t      cut_brackets(char *str);
size_t      cut_quots(char *str);
char        *cut_mirr(char **splitted);
char        *cook_mirr(char *new, char *add);
char        *join_mirr(char	*new, char *tmp, char *add);
void        construct_node(t_graph *g, char **dict, size_t *r);

//more comfortable form of type cast
# define TOK_TYPE ((t_tok *)(token_list->content))->type
# define TOK_VALUE ((t_tok *)(token_list->content))->value

# define INO input_not_over

//MISTAKES
short   parse_err;
short   syntax_err;
short   input_not_over;

//for INPUT_NOT_OVER and SYNTAX_ERR
# define PRO_SUBSH  0
# define PRO_PIPE   1
# define PRO_AND	2
# define PRO_OR     3
# define PRO_NONE   4
# define PRO_APOF   5
# define PRO_DQUOT  6
# define PRO_SQU	7
# define PRO_LAM	8

//for SYNTAX_ERR
# define PRO_IF     11
# define PRO_FOR	12
# define PRO_WHILE  13
# define PRO_DO     14
# define PRO_THEN   15
# define PRO_ELSE   16

#define STRTERR "syntax error or unexpected token in the beginning of a command"
#define NEWLERR "syntax error or unexpected token near new line"
#define SIMPERR "syntax error or unexpected token near '"
#define FENDERR "syntax error or unexpected token near the end of function"

//DEBUGGING
void		dbg_print_tokens(t_dlist *toklst);

#endif