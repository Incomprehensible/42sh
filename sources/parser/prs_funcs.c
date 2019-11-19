/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 17:59:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 09:26:14 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_errors.h"

static t_dlist	*prs_f_dup_tks(t_dlist *tks, t_dlist **fcode)
{
	t_tok		*tok;
	t_tok		*ntok;
	t_dlist		*instr;
	char		*val;

	while (tks && (tok = tks->content))
	{
		val = 0;
		if (!(ntok = ft_memalloc(sizeof(t_tok))))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		if (tok->value && !(val = ft_strdup(tok->value)))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		ntok->value = val;
		ntok->type = tok->type;
		if (!(instr = ft_dlstnew_cc(ntok, tks->size)))
			sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
		ft_dlstpush(fcode, instr);
		tks = tks->next;
		if (tok->type == TK_FEND)
			break ;
	}
	return (tks);
}

t_dlist			*prs_func(t_dlist *tks, ENV *envr)
{
	t_avln		*node;
	t_tok		*tok;
	t_func		*func;

	tks = tks->next;
	tok = tks->content;
	if (!(func = ft_memalloc(sizeof(FUNC))))
		sys_fatal_memerr("PARSING FAILED, MALLOC RETURNED 0");
	node = ft_avl_node_cc(tok->value, func, sizeof(FUNC));
	tks = tks->next;
	if (!(tks = prs_f_dup_tks(tks, &func->func_code)))
		return (0);
	ft_avl_set(envr->funcs, node);
	return (tks);
}
