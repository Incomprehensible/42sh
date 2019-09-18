/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_rdrs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:21:47 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 21:35:57 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char			*rdr_get_filename(t_tok *tok, ENV *envr)
{
	char		*ch;

	if ((TK_PROF_IN | TK_PROF_OUT) & tok->type)
		ch = prc_substitute(tok->value, envr, TK_PROF_IN == tok->type ? 1 : 0);
	else
		ch = ft_strdup(tok->value);
	return (ch);
}

REDIRECT		*prs_rdr_wa(t_dlist *tokens, ENV *envr)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || tks[0]->type != TK_FD)
		redir->fdl = 1;
	else if (tks[0] && tks[0]->type == TK_FD)
		redir->fdl = ft_atoi(tks[0]->value);
	if (tks[2] && tks[2]->type & TK_FLS_FDR && (redir->fdr == -1))
	{
		if (!(redir->file = rdr_get_filename(tks[2], envr)))
			free(redir);
		redir->fdr = 0;
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value);
	return (redir);
}

REDIRECT		*prs_rdr_r(t_dlist *tokens, ENV *envr)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || tks[0]->type != TK_FD)
		redir->fdl = 0;
	else if (tks[0] && tks[0]->type == TK_FD)
		redir->fdl = ft_atoi(tks[0]->value); // UNDONE: TO WORK WITH & (STDALL)
	if (tks[2] && tks[2]->type & TK_FLS_FDR && (redir->fdr == -1))
	{
		if (!(redir->file = rdr_get_filename(tks[2], envr)))
			free(redir);
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value); // UNDONE: TO WORK WITH &- (CLOSE)
	return (redir);
}

REDIRECT		*prs_rdr_rw(t_dlist *tokens, ENV *envr)
{
	REDIRECT	*redir;
	t_tok		*tks[3];

	tks[0] = tokens->prev ? tokens->prev->content : 0;
	tks[1] = tokens->content;
	tks[2] = tokens->next ? tokens->next->content : 0;
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tks[1]);
	if (!(tks[0]) || !(tks[1]))
	{
		free(redir);
	}
	else if (tks[0] && tks[0]->type == TK_FD)
		redir->fdl = ft_atoi(tks[0]->value);
	if (tks[2] && tks[2]->type & TK_FLS_FDR && (redir->fdr == -1))
	{
		if (!(redir->file = rdr_get_filename(tks[2], envr)))
			free(redir);
	}
	else if (tks[2] && tks[2]->type)
		redir->fdr = ft_atoi(tks[2]->value);
	return (redir);
}

t_dlist			*prs_new_rdr_cr(t_dlist *tokens, ENV *envr)
{
	t_tok		*tok;
	REDIRECT	*redir;
	t_dlist		*res;

	tok = tokens->content;
	redir = 0;
	redir = (tok->type == TK_RD_W || tok->type == TK_RD_A) \
		? prs_rdr_wa(tokens, envr) : redir;
	redir = (tok->type == TK_RD_R) ? prs_rdr_r(tokens, envr) : redir;
	redir = (tok->type == TK_RD_RW) ? prs_rdr_rw(tokens, envr) : redir;
	if (!redir || !(res = ft_dlstnew_cc(0, sizeof(REDIRECT))))
		return (0); // ERROR: prs_rdr: Malloc failed.
	res->content = redir;
	return (res);
}

t_dlist			*prs_rdrs(t_dlist **tokens, ENV *envr)
{
	t_dlist		*rdrs;
	t_tok		*tok;
	t_dlist		*it;
	t_dlist		*new_rdr;

	it = *tokens;
	rdrs = 0;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type >= TK_RD_W && tok->type <= TK_RD_RW)
		{
			if (!(new_rdr = prs_new_rdr_cr(it, envr)))
				it = 0; // ERROR: prs_rdr: Malloc failed.
			ft_dlstpush(&rdrs, new_rdr);
		}
		it = it->next;
	}
	*tokens = it;
	return (rdrs);
}
