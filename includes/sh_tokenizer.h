/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/08/19 00:53:23 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_42SH_SH_TOKENIZER_H
#define INC_42SH_SH_TOKENIZER_H

#include "sh_req.h"
#include "sh_token.h"

int		get_line(int fd, char **line);

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
t_stx       *init_quotes(t_stx *tree);
t_stx       *init_hedoc(t_stx *tree);
//t_stx       *init_eof(t_stx *tree);
char        *parse_sep(char *str, t_dlist **tok, short i);
//char        *parse_mirror(char *str, t_dlist **tok, t_stx **tr, short i);
char        *parse_quotes(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_scripts(char *str, t_dlist **tok, t_stx **tree, short choice);
char        *parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_math(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_envar(char *str, t_dlist **tok, t_stx **tree, short ind);
char        *parse_redir(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_func(char *str, t_dlist **tok, t_stx **tree, short i);
char        *parse_subsh(char *str, t_dlist **tok, t_stx **tree, short ind);
char        *parse_comm(char *str, t_dlist **tok, t_stx **tree, short i);
void        make_token(t_dlist **list, char *value, t_tk_type type);
int         special_meta(char meta);
char        *ft_process_s(char *str, char *end);
char        *ft_process_space(char *str, char *end);
char        *ft_process_seq(char *str, char *end);
char        *ft_process_any(char *str, char *meta, char *end);
char        *process_reg(char *str, char *meta, char *end);
char        *get_point(char *meta);
char        *block_pass(t_tk_type , char *str, t_dlist **toklst, t_stx **tree);
t_tk_type   find_token(t_stx **tree, char *str);
int         check_branch(char *str, t_stx *tree);
char        *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr);
short       is_token_here(char *start, char *meta);
short       input_finished(char *str, t_stx **tree, short path);
void        tree_init(t_stx **tree);
t_graph     *if_script_in(void);
t_graph     *until_script_in(void);
t_graph     *for_script_in(void);
t_graph     *while_script_in(void);
t_graph     *redir_in(void);
t_graph     *redir_two(void);
t_graph     *redir_one(void);
short       graph_type(t_graph *g, char *str);
short       pull_type(char *str);
char        *new_graph(t_graph *g, char *s, t_dlist **tok, t_stx **tr);
char        *check_subbranch(char *str,  t_dlist **tok, t_stx **tree, t_tk_type block);
short       graph_forward_only(t_graph *g);
short       graph_end(t_graph *g, char *str);
char        *pull_expr2(char *str, t_stx **tr, t_dlist **tok);
char        *pull_expr1(char *patt, char *str, t_stx **tr, t_dlist **tok);
size_t      layer_parse_two(char *meta, char *str);
char        *pull_token(char *str, size_t i);
char        *script_pull(char *patt, t_tk_type type, char *str, t_stx **tr, t_dlist **tok);
char        *reg_process(char *patt, t_tk_type type, char *str, t_stx **tr, t_dlist **tok);
char        *parse_empty(char *str, char *patt, t_dlist **tok);
char        *ft_process_vars(t_tk_type type, char *str, char *meta, t_dlist **tok);
short       is_separator(char str);
char        *get_deref(char *str, t_stx **tr, t_dlist **tok);
void        unexpected_token(void);

#endif