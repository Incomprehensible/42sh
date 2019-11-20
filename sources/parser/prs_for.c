/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_for.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 09:33:42 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 07:37:45 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include "sys_tools/sys_tools.h"

t_dlist			*prs_forin_loop_exec(t_dlist **tks_ds, \
ENV *env, int *sts, char **vals)
{
	size_t			ind[2];
	char			*key;
	t_dyn_string	*tmp;
	t_tok			*tok;

	ind[0] = 0;
	ind[1] = sizeof_warr(vals);
	key = (char *)tks_ds[2];
	while (ind[0] < ind[1])
	{
		tmp = dstr_new(vals[ind[0]]);
		if (env_set_variable(key, tmp, env) < 0)
			return (0);
		dstr_del(&tmp);
		if (!(tks_ds[0] = sh_tparse(tks_ds[1]->next, env, TK_DONE, sts)) ||
		tks_ds[0] == INTERRUPT_CALL)
			break ;
		if ((tok = tks_ds[0]->content)->type == TK_BREAK)
			break ;
		++ind[0];
	}
	et_rm_warr(vals);
	return (prs_skip_paired(tks_ds[1], TK_DO, TK_DONE));
}

t_dlist			*prs_forin(t_dlist *tks, ENV *env, int *sts)
{
	t_dlist			*loop_d[3];
	char			**vals;
	char			*tmp;
	t_tok			*tok;

	loop_d[2] = (t_dlist *)((t_tok *)tks->content)->value;
	tks = prs_skip_until(tks, TK_DO | TK_EXPR | TK_DEREF);
	if (!tks || !(tok = tks->content) || !(tok->type & (TK_EXPR | TK_DEREF)))
		return (0);
	if (!(tmp = prs_forin_vals(tks, env)))
		return (0);
	if (!(vals = ft_strsplits(tmp, " \t\n")))
		return (0);
	free(tmp);
	loop_d[1] = prs_skip_until(tks, TK_DO);
	loop_d[0] = tks;
	return (prs_forin_loop_exec(loop_d, env, sts, vals));
}

t_dlist			*prs_for_mt_conds(t_dlist *tks, ENV *env, \
int *sts, char **mt_conds)
{
	t_dlist		*loop_bg;
	t_tok		*tok;
	ERR			m_err;

	loop_bg = prs_skip_until(tks, TK_DO);
	m_err.err_code = 0;
	m_err.error_msg = NULL;
	do_math_bltn(mt_conds[0], env, &m_err);
	while (!m_err.err_code && do_math_bltn(mt_conds[1], env, &m_err))
	{
		if ((tks = sh_tparse(loop_bg->next, env, TK_DONE, sts)) \
		== INTERRUPT_CALL)
			return (INTERRUPT_CALL);
		if ((tok = tks->content)->type == TK_BREAK)
			break ;
		do_math_bltn(mt_conds[2], env, &m_err);
	}
	if (m_err.err_code)
		math_error_handler(&m_err, 0, env);
	et_rm_warr(mt_conds);
	return (prs_skip_paired(loop_bg, TK_DO, TK_DONE));
}

t_dlist			*prs_formt(t_dlist *tks, ENV *env, int *sts)
{
	char		**mt_conds;
	t_dlist		*loop_bg;

	if (!(mt_conds = ft_strsplit(((t_tok *)tks->content)->value, ';')))
		return (0);
	if (sizeof_warr(mt_conds) != 3)
		return (0);
	loop_bg = prs_skip_until(tks, ~(TK_EMPTY | TK_SEP));
	if (!loop_bg || !(loop_bg->content) || \
	!(((t_tok *)loop_bg->content)->type != TK_DO))
		return (0);
	return (prs_for_mt_conds(tks, env, sts, mt_conds));
}

t_dlist			*prs_for(t_dlist *tks, ENV *env, int *sts)
{
	t_dlist			*cond;
	t_tok			*tok;

	cond = prs_skip_until(tks, TK_DO | TK_MATH | TK_VAR);
	tok = cond->content;
	if (tok->type == TK_DO)
		return (0);
	if (tok->type == TK_VAR)
		return (prs_forin(cond, env, sts)->next);
	if (tok->type == TK_MATH)
		return (prs_formt(cond, env, sts)->next);
	return (0);
}
