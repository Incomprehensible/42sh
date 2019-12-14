/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:14:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:40:14 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"
#include "executer.h"

int				prs_is_a_instruction(t_tok *tok)
{
	if (tok && !(tok->type & (TK_SEPS | TK_FLOWS)))
		return (1);
	return (0);
}

t_rdtype		prs_rdr_type(t_tok *tok)
{
	if (tok->type == TK_RD_A)
		return (a_rdr);
	if (tok->type == TK_RD_R)
		return (r_rdr);
	if (tok->type == TK_RD_W)
		return (w_rdr);
	if (tok->type == TK_HERED)
		return (herd);
	return (rw_rdr);
}

t_dlist			*args_get_next_arg(t_dlist *tks)
{
	t_tok		*tok;

	while (tks && (tok = tks->content) && !(tok->type & \
	(TK_DEREF | TK_EXPR | TK_PROC_IN | TK_PROC_OUT | TK_MATH | TK_VALUE)))
	{
		tks = arg_tok_skip(tks, TK_FD | TK_EMPTY);
		if (tks && (tok = tks->content) && (tok->type & TK_RDS1))
		{
			tks = arg_tok_skip(tks, TK_RDS1 | TK_EMPTY);
			tks = arg_tok_skip(tks, TK_DEREF | TK_MATH | TK_NAME \
			| TK_SUBSH | TK_FILENAME);
		}
		if (tks && (tok = tks->content) && (tok->type & TK_HERED))
		{
			tks = arg_tok_skip(tks, TK_HERED | TK_EMPTY);
			tks = arg_tok_skip(tks, TK_WORD | TK_EMPTY);
		}
		if (tok->type & (TK_FLOWS | TK_SEPS | TK_EOF))
			return (tks);
	}
	return (tks);
}
