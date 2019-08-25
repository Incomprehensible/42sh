/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:14:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/25 03:20:15 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

int				prs_is_a_instruction(t_tok *tok)
{
	if (tok && tok->type != pipe_tk && \
	!(tok->type >= sep_tk && tok->type <= and_tk) && \
	tok->type != eof_tk && tok->type != if_tk && tok->type != else_tk && \
	tok->type != then_tk)
		return (1);
	return (0);
}

t_rdtype		prs_rdr_type(t_tok *tok)
{
	if (tok->type == rd_a_tk)
		return (a_rdr);
	if (tok->type == rd_r_tk)
		return (r_rdr);
	if (tok->type == rd_w_tk)
		return (w_rdr);
	return (rw_rdr);
}
