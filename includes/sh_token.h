/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/03 00:44:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

# include <unistd.h>

typedef size_t		t_tk_type;

# define TK_HERED		(t_tk_type)0x0000000000000001
# define TK_EMPTY		(t_tk_type)0x0000000000000002
# define TK_EXPR		(t_tk_type)0x0000000000000004
# define TK_SEP			(t_tk_type)0x0000000000000008
# define TK_OR			(t_tk_type)0x0000000000000010
# define TK_AND			(t_tk_type)0x0000000000000020
# define TK_BCKR_PS		(t_tk_type)0x0000000000000040
# define TK_RD_W		(t_tk_type)0x0000000000000080
# define TK_RD_R		(t_tk_type)0x0000000000000100
# define TK_RD_A		(t_tk_type)0x0000000000000200
# define TK_RD_RW		(t_tk_type)0x0000000000000400
# define TK_WORD		(t_tk_type)0x0000000000000800
# define TK_FD			(t_tk_type)0x0000000000001000
# define TK_FILENAME	(t_tk_type)0x0000000000002000
# define TK_PIPE		(t_tk_type)0x0000000000004000
# define TK_ASSIGM		(t_tk_type)0x0000000000008000
# define TK_NAME		(t_tk_type)0x0000000000010000
# define TK_VALUE		(t_tk_type)0x0000000000020000
# define TK_MATH		(t_tk_type)0x0000000000040000
# define TK_SUBSH		(t_tk_type)0x0000000000080000
# define TK_DEREF		(t_tk_type)0x0000000000100000
# define TK_IF			(t_tk_type)0x0000000000200000
# define TK_THEN		(t_tk_type)0x0000000000400000
# define TK_ELSE		(t_tk_type)0x0000000000800000
# define TK_FI			(t_tk_type)0x0000000001000000
# define TK_WHILE		(t_tk_type)0x0000000002000000
# define TK_DO			(t_tk_type)0x0000000004000000
# define TK_DONE		(t_tk_type)0x0000000008000000
# define TK_FOR			(t_tk_type)0x0000000010000000
# define TK_IN			(t_tk_type)0x0000000020000000
# define TK_BREAK		(t_tk_type)0x0000000040000000
# define TK_CONTIN		(t_tk_type)0x0000000080000000
# define TK_EXEC		(t_tk_type)0x0000000100000000
# define TK_FUNCTION	(t_tk_type)0x0000000200000000
# define TK_LAMBDA		(t_tk_type)0x0000000400000000
# define TK_RETURN		(t_tk_type)0x0000000800000000
# define TK_EOF			(t_tk_type)0x0000001000000000
# define TK_FEND		(t_tk_type)0x0000002000000000
# define TK_VAR			(t_tk_type)0x0000004000000000
# define TK_UNTIL		(t_tk_type)0x0000008000000000
# define TK_PROC_IN		(t_tk_type)0x0000010000000000
# define TK_PROC_OUT	(t_tk_type)0x0000020000000000
# define TK_PROF_IN		(t_tk_type)0x0000040000000000
# define TK_PROF_OUT	(t_tk_type)0x0000080000000000
# define TK_ARSHLOCH	(t_tk_type)0x0000100000000000

# define TK_SEPS		(t_tk_type)(0x1000004038 | TK_ARSHLOCH)
# define TK_FLOWS		(t_tk_type)0x90ffe00000
# define TK_FDS_RDS		(t_tk_type)0x0c0000003780

# define TK_RDS1		(t_tk_type)(TK_RD_W | TK_RD_R | TK_RD_A | TK_RD_RW)
# define TK_RDS			(t_tk_type)(TK_HERED | TK_RDS1)

# define TK_FLS_FDR		(t_tk_type)(TK_PROF_IN | TK_PROF_OUT | TK_FILENAME)

# define TK_SEPS1		(TK_SEPS & ~TK_PIPE)

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
