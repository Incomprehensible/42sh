/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_if_while.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:03:53 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 06:56:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_tools.h"

static t_dlist	*prs_skip_after_else(t_dlist *tks)
{
	t_tok		*tok;

	while (tks && (tok = tks->content))
	{
		if (tok->type == TK_IF)
			tks = prs_skip_paired_trg(tks->next, TK_IF, TK_FI, -1)->next;
		else if (tok->type == TK_ELSE || tok->type == TK_FI)
			return (tks);
		else
			tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_if(t_dlist *tks, ENV *env, int *sts)
{
	t_tok			*tok;

	tks = sh_tparse(tks->next, env, TK_THEN, sts);
	if (*sts == EXIT_SUCCESS && tks != INTERRUPT_CALL)
	{
		if ((tks = sh_tparse(tks->next, env, \
		TK_ELSE | TK_FI, sts)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		tok = tks->content;
		if (tok->type & (TK_BREAK | TK_CONTIN))
			return (tks);
		tks = prs_skip_paired_trg(tks, TK_IF, TK_FI, -1);
	}
	else if (tks != INTERRUPT_CALL)
	{
		tks = prs_skip_after_else(tks);
		if ((tok = tks->content)->type == TK_FI)
			return (tks->next);
		if ((tks = sh_tparse(tks->next, env, TK_FI, sts)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		if ((tok = tks->content)->type == TK_BREAK)
			return (tks);
	}
	return (tks);
}

t_dlist			*prs_while(t_dlist *tks, ENV *env, int *sts)
{
	const t_dlist	*cond = tks->next;
	t_dlist			*end;
	t_tok			*tok;

	while ((end = sh_tparse((t_dlist *)cond, env, TK_DO, sts)) \
	&& *sts == EXIT_SUCCESS && end != INTERRUPT_CALL)
	{
		if ((tks = sh_tparse(end->next, env, TK_DONE, sts)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			break ;
	}
	return (prs_skip_paired(end, TK_DO, TK_DONE)->next);
}

char			*prs_forin_vals(t_dlist *tks, ENV *env)
{
	t_tok			*tok;
	t_dyn_string	*valbuff;
	char			*tmp;
	char			*res;

	valbuff = dstr_new("");
	while (tks && (tok = tks->content) && (tok->type & (TK_EXPR | TK_DEREF)))
	{
		if (!(tks = arg_sub(tks, &tmp, 0, env)))
			return (0);
		dstr_insert_str(valbuff, " ", valbuff->strlen);
		dstr_insert_str(valbuff, tmp, valbuff->strlen);
		free(tmp);
		tks = prs_skip_until(tks, TK_DO | TK_EXPR | TK_DEREF);
	}
	res = ft_strdup(valbuff->txt);
	dstr_del(&valbuff);
	return (res);
}

size_t			sizeof_warr(char **args)
{
	size_t		ind;

	ind = 0;
	while (args[ind])
		++ind;
	return (ind);
}
