/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_skipers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:51:18 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 17:54:04 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_dlist			*prs_skip_paired_trg(t_dlist *tks, t_tk_type op, t_tk_type cls, int cntr)
{
	t_tok		*tok;

	while (tks && (tok = tks->content))
	{
		if (tok->type & op)
			--cntr;
		if (tok->type & cls)
			++cntr;
		if (!cntr)
			break ;
		tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_skip_until(t_dlist *tks, t_tk_type brk_toks)
{
	t_tok		*tok;

	while (tks && (tok = tks->content))
		if (tok->type & brk_toks)
			return (tks);
		else
			tks = tks->next;
	return (0);
}

t_dlist			*prs_skip_paired(t_dlist *tks, t_tk_type op, t_tk_type cls)
{
	int			cntr;
	t_tok		*tok;

	cntr = 0;
	while (tks && (tok = tks->content))
	{
		if (tok->type & op)
			--cntr;
		if (tok->type & cls)
			++cntr;
		if (!cntr)
			break ;
		tks = tks->next;
	}
	return (tks);
}
