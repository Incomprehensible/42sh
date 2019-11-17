/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_if_while.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:03:53 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/17 08:32:41 by bomanyte         ###   ########.fr       */
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
			return(tks);
		else
			tks = tks->next;
	}
	return (tks);
}

t_dlist			*prs_if(t_dlist *tks, ENV *envs, int *status)
{
	t_tok			*tok;

	tks = sh_tparse(tks->next, envs, TK_THEN, status);
	if (*status == EXIT_SUCCESS && tks != INTERRUPT_CALL)
	{
		if ((tks = sh_tparse(tks->next, envs, TK_ELSE | TK_FI, status)) == INTERRUPT_CALL)
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
		if ((tks = sh_tparse(tks->next, envs, TK_FI, status)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		if ((tok = tks->content)->type == TK_BREAK)
			return (tks);
	}
	return (tks);
}

t_dlist			*prs_while(t_dlist *tks, ENV *envs, int *status)
{
	const t_dlist	*cond = tks->next;
	t_dlist			*end;
	t_tok			*tok;

	while ((end = sh_tparse((t_dlist *)cond, envs, TK_DO, status)) \
	&& *status == EXIT_SUCCESS && end != INTERRUPT_CALL)
	{
		if ((tks = sh_tparse(end->next, envs, TK_DONE, status)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		tok = tks->content;
		if (tok->type == TK_BREAK)
			break ;
	}
	return (prs_skip_paired(end, TK_DO, TK_DONE)->next);
}

char 			*prs_forin_vals(t_dlist *tks, ENV *envs)
{
	t_tok		*tok;
	DSTRING		*valbuff;
	char		*tmp;
	char		*res;

	valbuff = dstr_new("");
	while (tks && (tok = tks->content) && (tok->type & (TK_EXPR | TK_DEREF)))
	{
		if (!(tks = arg_sub(tks, &tmp, 0, envs)))
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

t_dlist			*prs_forin_loop_exec(t_dlist **tks_ds, ENV *envs, int *status, char **vals)
{
	size_t		ind[2];
	char		*key;
	DSTRING		*tmp;
	t_tok		*tok;

	ind[0] = 0;
	ind[1] = sizeof_warr(vals);
	key = (char *)tks_ds[2];
	while (ind[0] < ind[1])
	{
		tmp = dstr_new(vals[ind[0]]);
		if (env_set_variable(key, tmp, envs) < 0)
			return (0); // ERROR
		dstr_del(&tmp);
		if (!(tks_ds[0] = sh_tparse(tks_ds[1]->next, envs, TK_DONE, status)) ||
		tks_ds[0] == INTERRUPT_CALL)
			break ;
		if ((tok = tks_ds[0]->content)->type == TK_BREAK)
			break ;
		++ind[0];
	}
	et_rm_warr(vals);
	return (prs_skip_paired(tks_ds[1], TK_DO, TK_DONE));
}

t_dlist 		*prs_forin(t_dlist *tks, ENV *envs, int *status)
{
	t_dlist			*loop_d[3];
	char			**vals;
	char			*key;
	char			*tmp;
	t_tok			*tok;

	loop_d[2] = (t_dlist *)((t_tok *)tks->content)->value;
	tks = prs_skip_until(tks, TK_DO | TK_EXPR | TK_DEREF);
	if (!tks || !(tok = tks->content) || !(tok->type & (TK_EXPR | TK_DEREF)))
		return (0);
	if (!(tmp = prs_forin_vals(tks, envs)))
		return (0);
	if (!(vals = ft_strsplits(tmp , " \t\n")))
		return (0);
	free(tmp);
	loop_d[1] = prs_skip_until(tks, TK_DO);
	loop_d[0] = tks;
	return (prs_forin_loop_exec(loop_d, envs, status, vals));
}

t_dlist			*prs_for_mt_conds(t_dlist *tks, ENV *envs, int *status, char **mt_conds)
{
	t_dlist		*loop_bg;
	t_tok		*tok;
	ERR			m_err;

	loop_bg = prs_skip_until(tks, TK_DO);
	m_err.err_code = 0;
	m_err.error_msg = NULL;
	do_math_bltn(mt_conds[0], envs, &m_err);
	while (!m_err.err_code && do_math_bltn(mt_conds[1], envs, &m_err))
	{
		if ((tks = sh_tparse(loop_bg->next, envs, TK_DONE, status)) == INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		if ((tok = tks->content)->type == TK_BREAK)
			break ;
		do_math_bltn(mt_conds[2], envs, &m_err);
	}
	if (m_err.err_code)
		math_error_handler(&m_err, 0, envs); // NADYA PUT HERE
	et_rm_warr(mt_conds);
	return (prs_skip_paired(loop_bg, TK_DO, TK_DONE));
}

t_dlist			*prs_formt(t_dlist *tks, ENV *envs, int *status)
{
	char		**mt_conds;
	t_dlist		*loop_bg;
	t_tok		*tok;

	if (!(mt_conds = ft_strsplit(((t_tok *)tks->content)->value, ';')))
		return (0);
	if (sizeof_warr(mt_conds) != 3)
		return (0);
	loop_bg = prs_skip_until(tks, ~(TK_EMPTY | TK_SEP));
	if (!loop_bg || !(loop_bg->content) || \
	!(((t_tok *)loop_bg->content)->type != TK_DO))
		return (0);
	return (prs_for_mt_conds(tks, envs, status, mt_conds));
}

t_dlist			*prs_for(t_dlist *tks, ENV *envs, int *status)
{
	t_dlist			*cond;
	t_tok			*tok;

	cond = prs_skip_until(tks, TK_DO | TK_MATH | TK_VAR);
	tok = cond->content;
	if (tok->type == TK_DO)
		return (0);
	if (tok->type == TK_VAR)
		return (prs_forin(cond, envs, status)->next);
	if (tok->type == TK_MATH)
		return (prs_formt(cond, envs, status)->next);
	return (0);
}
