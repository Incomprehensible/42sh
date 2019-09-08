/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/08 11:38:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

int				prs_deref_name(t_dlist **tks, ENV *envs)
{
	t_tok		*dtok;
	t_dlist		*tmp;
	DSTRING		*dstr;
	int			ret;

	tmp = (*tks)->next;
	ret = 0;
	ft_dlst_delcut(tks, free_token);
	(*tks) = tmp;
	dtok = tmp->content;
	dtok->type = value_tk;
	if (!(dstr = env_get_variable(dtok->value, envs)))
		return (-1);
	free(dtok->value);
	if (!(dtok->value = ft_strdup(dstr->txt)))
		ret = -1; // ERROR: prs_deref: Malloc failed.
	dstr_del(&dstr);
	return (ret);
}

int				prs_deref_math(t_dlist **tks, ENV *envs)
{
	t_tok		*dtok;
	char		**exprs;
	t_dlist		*tmp;
	long		res;
	int			ret;

	tmp = (*tks)->next;
	ret = 0;
	ft_dlst_delcut(tks, free_token);
	(*tks) = tmp;
	dtok = tmp->content;
	dtok->type = value_tk;
	exprs = ft_strsplits(dtok->value, ",");
	// TODO: unsafe malloc.
	while (exprs[ret])
		res = do_math_bltn(exprs[ret++], envs);
	et_rm_warr(exprs);
	free(dtok->value);
	if (!(dtok->value = ft_lltoa_base(res, 10)))
		ret = -1; // ERROR: prs_deref: Malloc failed.
	return (0);
}

int				prs_vars_derefs(t_dlist *tks, ENV *envs)
{
	t_tok		*tok;
	t_dlist		*it;

	it = tks;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == deref_tk)
		{
			tok = it->next->content;
			if (tok->type == name_tk && (prs_deref_name(&it, envs) < 0))
				return (-1); // ERROR: prs_deref: Malloc failed.
			if (tok->type == math_tk && (prs_deref_math(&it, envs) < 0))
				return (-1); // ERROR: prs_deref: Malloc failed.
		}
		it = it->next;
	}
	return (0);
}

int				prs_join_exprs(t_dlist *tks)
{
	t_tok		*tok[2];
	t_dlist		*tmp;
	char		*newval;

	tok[0] = tks->content;
	tok[1] = tks->next->content;
	tmp = tks->next;
	if (!(newval = ft_strjoin(tok[0]->value, tok[1]->value)))
		return (-1);
	ft_dlst_delcut(&tmp, free_token);
	free(tok[0]->value);
	tok[0]->value = newval;
	tok[0]->type = expr_tk;
	return (1);
}

int				prs_values_joins(t_dlist *tks)
{
	t_tok		*tok;
	t_tok		*ntok;
	t_dlist		*ntks;

	while (tks && tks->next && prs_is_a_instruction((tok = tks->content)))
	{
		ntks = tks->next;
		ntok = ntks->content;
		if (tok->type == value_tk && (ft_strlen(tok->value) == 0))
		{
			ft_dlst_delcut(&tks, free_token);
			tks = ntks;
		}
		else if ((tok->type == expr_tk || tok->type == value_tk) \
		&& (ntok->type == expr_tk || ntok->type == value_tk))
		{
			if (prs_join_exprs(tks) < 0)
				return (-1);
		}
		else if (tok->type == value_tk)
		{
			tok->type = expr_tk;
		}
		else
			tks = tks->next;
	}
	return (0);
}

char			**prs_args(t_dlist *tokens, ENV *envs)
{
	char		**args;
	size_t		argslen;
	size_t		i;
	t_dlist		*it;
	t_tok		*tok;

	argslen = 0;
	prs_vars_derefs(tokens, envs); // TODO: Malloc checking in prs_args.
	prs_values_joins(tokens);
	it = tokens;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		tok->type == expr_tk ? ++argslen : 0;
		it = it->next;
	}
	if (!(args = ft_memalloc(sizeof(char*) * (argslen + 1))))
		return (0); // ERROR: prs_args: Malloc failed.
	it = tokens;
	i = -1;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == expr_tk)
			if (!(args[++i] = ft_strdup(tok->value))) // FEATURE: prs_args: Возможное место для обхода экранирования.
				return (0); // ERROR: prs_args: Malloc failed. (LEAKS alert).
		it = it->next;
	}
	return (args);
}
