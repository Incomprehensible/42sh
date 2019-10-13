/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:14:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/12 22:24:24 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

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
