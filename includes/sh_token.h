/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/09 23:00:31 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

# include <unistd.h>

typedef size_t		t_tk_type;

#define TK_EMPTY	(t_tk_type)0x0000000000000002 // 2
#define TK_EXPR		(t_tk_type)0x0000000000000004 // 4
#define TK_SEP		(t_tk_type)0x0000000000000008 // 8
#define TK_OR		(t_tk_type)0x0000000000000010 // 16
#define TK_AND		(t_tk_type)0x0000000000000020 // 32
#define TK_BCKR_PS	(t_tk_type)0x0000000000000040 // 64
#define TK_RD_W		(t_tk_type)0x0000000000000080 // 128
#define TK_RD_R		(t_tk_type)0x0000000000000100 // 256
#define TK_RD_A		(t_tk_type)0x0000000000000200 // 512
#define TK_RD_RW	(t_tk_type)0x0000000000000400 // 1 024
#define TK_HERED	(t_tk_type)0x0000000000000001 // 1
#define TK_WORD		(t_tk_type)0x0000000000000800 // 2 048
#define TK_FD		(t_tk_type)0x0000000000001000 // 4 096
#define TK_FILENAME	(t_tk_type)0x0000000000002000 // 8 192
#define TK_PIPE		(t_tk_type)0x0000000000004000 // 16 384
#define TK_ASSIGM	(t_tk_type)0x0000000000008000 // 32 768
#define TK_NAME		(t_tk_type)0x0000000000010000 // 65 536
#define TK_VALUE	(t_tk_type)0x0000000000020000 // 131 072
#define TK_MATH		(t_tk_type)0x0000000000040000 // 262 144
#define TK_SUBSH	(t_tk_type)0x0000000000080000 // 524 288
#define TK_DEREF	(t_tk_type)0x0000000000100000 // 1 048 576
#define TK_IF		(t_tk_type)0x0000000000200000 // 2 097 152
#define TK_THEN		(t_tk_type)0x0000000000400000 // 4 194 304
#define TK_ELSE		(t_tk_type)0x0000000000800000 // 8 388 608
#define TK_FI		(t_tk_type)0x0000000001000000 // 16 777 216
#define TK_WHILE	(t_tk_type)0x0000000002000000 // 33 554 432
#define TK_DO		(t_tk_type)0x0000000004000000 // 67 108 864
#define TK_DONE		(t_tk_type)0x0000000008000000 // 134 217 728
#define TK_FOR		(t_tk_type)0x0000000010000000 // 268 435 456
#define TK_IN		(t_tk_type)0x0000000020000000 // 536 870 912
#define TK_BREAK	(t_tk_type)0x0000000040000000 // 1 073 741 824
#define TK_CONTIN	(t_tk_type)0x0000000080000000 // 2 147 483 648
#define TK_EXEC		(t_tk_type)0x0000000100000000 // 4 294 967 296
#define TK_FUNCTION	(t_tk_type)0x0000000200000000 // 8 589 934 592
#define TK_LAMBDA	(t_tk_type)0x0000000400000000 // 17 179 869 184 !DU
#define TK_RETURN	(t_tk_type)0x0000000800000000 // 34 359 738 368 !DU
#define TK_EOF		(t_tk_type)0x0000001000000000 // 68 719 476 736
#define TK_FEND		(t_tk_type)0x0000002000000000 // 137 438 953 472
#define TK_VAR		(t_tk_type)0x0000004000000000 // 274 877 906 944
#define TK_UNTIL	(t_tk_type)0x0000008000000000 // 549 755 813 888
#define TK_PROC_IN	(t_tk_type)0x0000010000000000 // 1 099 511 627 776
#define TK_PROC_OUT	(t_tk_type)0x0000020000000000 // 2 199 023 255 552
#define TK_PROF_IN	(t_tk_type)0x0000040000000000 // 4 398 046 511 104
#define TK_PROF_OUT	(t_tk_type)0x0000080000000000 // 8 796 093 022 208

#define TK_SEPS		(t_tk_type)0x1000004038 // SEP + PIPE + EOF
#define TK_FLOWS	(t_tk_type)0x90ffe00000 // [IF -> CONTIN] + UNTIL + EOF
#define TK_FDS_RDS	(t_tk_type)0x0c0000003780

#define TK_RDS		(t_tk_type)0x781		// RDS + HERED

#define TK_FLS_FDR	(t_tk_type)(TK_PROF_IN | TK_PROF_OUT | TK_FILENAME)

#define TK_SEPS1	(TK_SEPS & ~TK_PIPE) // SEP + OR + AND + PIPE + EOF

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
