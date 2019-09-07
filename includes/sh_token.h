/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/15 16:41:11 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

typedef enum		e_tk_type
{
	empty_tk, // Empty token
	cmd_tk, // cmd or arg
	sep_tk, // ;
	pipe_tk, // pipe |
	expr_tk,
	or_tk,
	and_tk,
	bckr_ps_tk,
	rd_w_tk,
	rd_r_tk,
	rd_a_tk,
	rd_rw_tk,
	hered_tk,
	word_tk,
    fd_tk,
    filename_tk,
    assigm_tk,
    name_tk,
    value_tk,
    math_tk,
    subsh_tk,
    deref_tk,
    if_tk,
    then_tk,
    else_tk,
    fi_tk,
    while_tk,
    do_tk,
    done_tk,
    for_tk,
    in_tk,
    until_tk,
    exec_tk,
    function_tk,
    apof_tk,
    dquote_tk,
    fend_tk,
    var_tk,
	eof_tk // end of file input
}					t_tk_type;

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

typedef struct      s_sstx
{
    char            *blck_name;
    struct s_stx     *blck_next; //ring list
    char            *tk_name;
    char            *tcontent;
    struct s_stx     *tk_next;
    struct s_stx     *next;
    struct s_stx     *prev;
    struct s_stx     *blck_point;
    struct s_stx     *denominator;
}                   t_sstx;

typedef struct      s_stx
{
    struct s_stx *next;
    struct s_stx *prev;
    char         *meta;
}                   t_stx;

#endif
