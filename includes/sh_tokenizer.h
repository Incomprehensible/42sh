/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tokenizer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/07 06:14:17 by bomanyte         ###   ########.fr       */
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
# define REDIR (short)0x0000000008
# define FUNCS (short)0x0000000009
# define EXPRS (short)0xB
# define DEREF (short)0xC
# define EMPTY (short)0x0000000013
# define HOLE (short)0x0000000014
# define PROF (short)0x0000000066
# define CURLY (short)0x0000000067

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
int         layer_parse_one(char *meta, char *str);
size_t      layer_parse_two(char *meta, char *str);
char        *pull_token(char *str, size_t i);
char        *script_pull(char *patt, t_tk_type type, char *str, t_stx **tr, t_dlist **tok);
char        *reg_process(char *patt, t_tk_type type, char *str, t_dlist **tok, t_stx **tr);
char        *ft_process_vars(t_tk_type type, char *str, char *meta, t_dlist **tok);
char        *parse_str_block(char *str, t_dlist **tok, t_stx **tree, short br);
short       is_separator(char str);
short       unexpected_token(void);
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

//more comfortable form of type cast
# define TOK_TYPE ((t_tok *)(token_list->content))->type
# define TOK_VALUE ((t_tok *)(token_list->content))->value

#endif