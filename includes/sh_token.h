/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/07 19:50:54 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

#include <string.h>

typedef size_t t_tk_type;

# define TK_SEPS (t_tk_type)0x10000004038
# define TK_MATHS (t_tk_type)0x0000000003
# define TK_QUOTS (t_tk_type)0x0000000004
# define TK_EXPRS (t_tk_type)0x0000000008
# define TK_FUNCS (t_tk_type)0x0000000006
# define TK_SUBSHS (t_tk_type)0x0000000007
# define TK_FLOWS (t_tk_type)0x0000000000
# define TK_EMPTY (t_tk_type)0x0000000000000002
# define TK_EXPR (t_tk_type)0x0000000000000004
# define TK_SEP (t_tk_type)0x0000000000000008
# define TK_OR (t_tk_type)0x0000000000000010
# define TK_AND (t_tk_type)0x0000000000000020
# define TK_BCKR_PS (t_tk_type)0x0000000000000040
# define TK_RD_W (t_tk_type)0x0000000000000080
# define TK_RD_R (t_tk_type)0x0000000000000100
# define TK_RD_A (t_tk_type)0x0000000000000200
# define TK_RD_RW (t_tk_type)0x0000000000000400
# define TK_HERED (t_tk_type)0x0000000000000001
# define TK_WORD (t_tk_type)0x0000000000000800
# define TK_FD (t_tk_type)0x0000000000001000
# define TK_FILENAME (t_tk_type)0x0000000000002000
# define TK_PIPE (t_tk_type)0x0000000000004000
# define TK_ASSIGM (t_tk_type)0x0000000000008000
# define TK_NAME (t_tk_type)0x0000000000010000
# define TK_VALUE (t_tk_type)0x0000000000020000
# define TK_MATH (t_tk_type)0x0000000000040000
# define TK_SUBSH (t_tk_type)0x0000000000080000
# define TK_DEREF (t_tk_type)0x0000000000100000
# define TK_IF (t_tk_type)0x0000000000200000
# define TK_THEN (t_tk_type)0x0000000000400000
# define TK_ELSE (t_tk_type)0x0000000000800000
# define TK_FI (t_tk_type)0x0000000001000000
# define TK_WHILE (t_tk_type)0x0000000002000000
# define TK_DO (t_tk_type)0x0000000004000000
# define TK_DONE (t_tk_type)0x0000000008000000
# define TK_FOR (t_tk_type)0x0000000010000000
# define TK_IN (t_tk_type)0x0000000020000000
# define TK_BREAK (t_tk_type)0x0000000040000000
# define TK_CONTIN (t_tk_type)0x0000000080000000
# define TK_EXEC (t_tk_type)0x0000000100000000
# define TK_FUNCTION (t_tk_type)0x0000000200000000
# define TK_APOF (t_tk_type)0x0000000400000000
# define TK_DQUOTE (t_tk_type)0x0000000800000000
# define TK_EOF (t_tk_type)0x0000010000000000
# define TK_FEND (t_tk_type)0x0000020000000000
# define TK_VAR (t_tk_type)0x0000040000000000
# define TK_UNTIL (t_tk_type)0x0000080000000000

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

typedef struct      s_stx
{
    struct s_stx *next;
    struct s_stx *prev;
    char         *meta;
}                   t_stx;

#endif
