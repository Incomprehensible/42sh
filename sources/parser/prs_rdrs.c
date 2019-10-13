/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_rdrs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:21:47 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/12 22:33:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

char			*rdr_get_filename(t_tok *tok, ENV *envr)
{
	char		*ch;

	if ((TK_PROF_IN | TK_PROF_OUT) & tok->type)
		ch = prc_substitute(tok->value, envr, TK_PROF_IN == tok->type ? 1 : 0);
	else
		ch = ft_strdup(tok->value);
	return (ch);
}

int				prs_rdr_fdl(t_dlist *tokens, REDIRECT *redir)
{
	t_tok		*tok;

	tokens = arg_tok_r_skip(tokens, TK_EMPTY | TK_RDS);
	if (!tokens || !(tok = tokens->content) || tok->type != TK_FD)
	{
		redir->fdl = redir->type == r_rdr ? 0: redir->fdl;
		redir->fdl = redir->type & (a_rdr | w_rdr) ? 1: redir->fdl;
		return (0);
	}
	else if (tok->type == TK_FD)
	{
		redir->fdl = ft_atoi(tok->value);
		return (0);
	}
	return (-1);
}

int				prs_rdr_fdr_file(t_dlist *tokens, REDIRECT *redir, ENV *envr)
{
	t_tok		*tok;

	if (!(tokens = arg_tok_skip(tokens, TK_EMPTY | TK_RDS)))
		return (-2);
	tok = tokens->content;
	if (tok->type & (TK_PROF_IN | TK_PROF_OUT | TK_FILENAME))
	{
		redir->file = rdr_get_filename(tok, envr);
		return (0);
	}
	if (tok->type == TK_FD)
	{
		redir->file = 0;
		redir->fdr = ft_atoi(tok->value);
		return (0);
	}
	return (-1);
}

int				prs_hrd_word(t_dlist *tl, REDIRECT *redr)
{
	t_tok		*tok;

	if (!(tl = arg_tok_skip(tl->next, TK_EMPTY)))
		return (-2);
	tok = tl->content;
	if (tok->type != TK_WORD)
		return (-1);
	if (!(redr->file = ft_strdup(tok->value)))
		sys_fatal_memerr("FAILED_MAKE_HEREDOC");
	redr->fdr = 0;
	redr->fdl = 0;
	return (0);
}

t_dlist			*prs_new_rdr_cr(t_dlist *tokens, ENV *envr)
{
	t_dlist		*res;
	REDIRECT	*redir;

	if (!(res = ft_dlstnew_cc(0, 0)))
		return (0);
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		return (0);
	redir->type = prs_rdr_type(tokens->content);
	if (redir->type != herd)
	{
		prs_rdr_fdr_file(tokens, redir, envr);
		prs_rdr_fdl(tokens, redir);
	}
	else
		prs_hrd_word(tokens, redir);
	res->content = redir;
	res->size = sizeof(REDIRECT);
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
	while (it)
	{
		if (!(it = prs_skip_until(it, TK_SEPS | TK_FLOWS | TK_RDS)))
			return (0);
		if ((tok = (it->content)) && (tok->type & TK_RDS))
		{
			new_rdr = prs_new_rdr_cr(it, envr);
			ft_dlstpush(&rdrs, new_rdr);
			it = it->next;
		}
		else
			break ;
	}
	*tokens = it;
	return (rdrs);
}
