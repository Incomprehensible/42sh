/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_and_or.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:54:50 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 12:55:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern void			DBG_PRINT_TOKENS(t_dlist *toklst);

static t_dlist	*prs_skip_flows(t_dlist *tks, t_tk_type type)
{
	t_tok		*tok;
	int			cicles;
	int			ifs;

	ifs = 0;
	cicles = 0;
	while (tks && (tok = tks->content))
	{
		(tok->type == TK_DO) ? ++cicles : 0;
		(tok->type == TK_DONE) ? --cicles : 0;
		(tok->type == TK_IF) ? ++ifs : 0;
		(tok->type == TK_FI) ? --ifs : 0;
		if (!cicles && !ifs && (tok->type & (TK_SEP | TK_EOF | type)))
			return (tks);
		tks = tks->next;
	}
	return (0);
}

t_dlist			*prs_and(t_dlist *tks, ENV *envr, int *status)
{
	if (*status == EXIT_SUCCESS)
	{
		tks = sh_tparse(tks->next, envr, TK_SEP | TK_EOF | TK_OR, status);
		return (tks);
	}
	*status = EXIT_FAILURE;
	return (prs_skip_flows(tks->next, TK_OR));
}

t_dlist			*prs_or(t_dlist *tks, ENV *envr, int *status)
{
	if (*status != EXIT_SUCCESS)
	{
		tks = sh_tparse(tks->next, envr, TK_SEP | TK_EOF | TK_AND, status);
		return (tks);
	}
	*status = EXIT_SUCCESS;
	return (prs_skip_flows(tks->next, TK_AND));
}
