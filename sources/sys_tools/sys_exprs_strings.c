/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_exprs_strings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 19:52:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:19:16 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"

/*
** Здесь будут функции получения аргументов командной строки для процессов.
** Получение аргументов производится путем парсинга структур для
** исполняемой таблицы!
*/

char		*ilget_subsh(SUBSH *sbh)
{
	DSTRING		*buff;
	char		*res;

	if (!(buff = dstr_new("")))
		sys_fatal_memerr("ILGET_SUBSH");
	if (sbh->ipipe_fds)
		dstr_insert_str(buff, "| ", buff->strlen);
	dstr_insert_str(buff, "( ", buff->strlen);
	dstr_insert_str(buff, sbh->commands, buff->strlen);
	dstr_insert_str(buff, " )", buff->strlen);
	res = ilget_rdrs(sbh->redirections);
	dstr_insert_str(buff, res, buff->strlen);
	free(res);
	res = ft_strdup(buff->txt);
	dstr_del(&buff);
	return (res);
}

char		*ilget_bkgr(BCKGR *bkgr)
{
	char		*res;
	char		*tmp;

	if (!(tmp = str_from_n_tkls(bkgr->tls, TOKSTR_LMT, 0, TK_ARSHLOCH)))
		if (!(tmp = ft_strdup(TOKSTR_FAIL)))
			sys_fatal_memerr("ILGET_BKGR");
	if (!(res = ft_strjoin(tmp, "&")))
		res = tmp;
	else
		free(tmp);
	return (res);
}

DSTRING		*ilget_args(EXPRESSION *xp)
{
	char		**av;
	DSTRING		*buff;
	ssize_t		i;

	i = -1;
	buff = dstr_new("");
	if (!buff || !xp)
		return (0);
	av = xp->args;
	while (av[++i])
	{
		dstr_insert_str(buff, av[i], buff->strlen);
		if (av[i + 1])
			dstr_insert_ch(buff, ' ', buff->strlen);
	}
	return (buff);
}

char		*ilget_expr(EXPRESSION *xpr)
{
	char		*res;
	DSTRING		*buff;
	DSTRING		*args;

	if (!(buff = dstr_new("")))
		sys_fatal_memerr("ILGET_XP");
	if (xpr->ipipe_fds)
		dstr_insert_str(buff, "| ", buff->strlen);
	if (!(args = ilget_args(xpr)))
		sys_fatal_memerr("ILGET_XP");
	dstr_insert_dstr(buff, args, buff->strlen);
	dstr_del(&args);
	if (!(res = ilget_rdrs(xpr->redirections)))
		sys_fatal_memerr("ILGET_XP");
	dstr_insert_str(buff, res, buff->strlen);
	free(res);
	if (!(res = ft_strdup(buff->txt)))
		sys_fatal_memerr("ILGET_XP");
	dstr_del(&buff);
	return (res);
}

char		*sys_get_str_from_etab(ETAB **etab)
{
	char	*input;

	if ((*etab)->type == ET_SUBSH)
		input = ilget_subsh((*etab)->instruction);
	else if ((*etab)->type == ET_BCKGR)
		input = ilget_bkgr((*etab)->instruction);
	else if ((*etab)->type == ET_EXPR)
		input = ilget_expr((*etab)->instruction);
	else
		input = 0;
	return (input);
}
