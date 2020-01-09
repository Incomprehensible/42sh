/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_rdrs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:21:47 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 09:44:42 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

t_dlist			*prs_new_rdr_cr(t_dlist *tokens, ENV *envr)
{
	t_dlist		*res;
	t_redir		*redir;
	int			err;

	if (!(res = ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("LIST_RDR_FAILED");
	if (!(redir = ft_memalloc(sizeof(REDIRECT))))
		sys_fatal_memerr("MEMALLOC_CREAT_FAILED");
	redir->type = prs_rdr_type(tokens->content);
	if (redir->type >= r_rdr && redir->type < herd)
	{
		err = prs_rdr_fdr_file(tokens, redir, envr);
		err = err >= 0 ? prs_rdr_fdl(tokens, redir) : err;
	}
	else if (redir->type == herd)
		err = prs_hrd_word(tokens, redir);
	else
		return (0);
	if (err != 0)
		return (0);
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
		if (!(it = prs_skip_until(it, TK_SEPS | TK_FLOWS | TK_RDS)))
			return (0);
		else if ((tok = (it->content)) && (tok->type & TK_RDS))
		{
			if ((new_rdr = prs_new_rdr_cr(it, envr)) == 0)
			{
				ft_dlst_delf(&rdrs, 0, et_rm_rdr);
				return (0);
			}
			ft_dlstpush(&rdrs, new_rdr);
			it = it->next;
		}
		else
			break ;
	*tokens = it;
	return (rdrs);
}
