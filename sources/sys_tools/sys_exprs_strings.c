/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_exprs_strings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 19:52:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/13 22:11:03 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"

/*
** Здесь будут функции получения аргументов командной строки для процессов.
** Получение аргументов производится путем парсинга структур для
** исполняемой таблицы!
*/

int			ilput_fdl(DSTRING *buff, REDIRECT *rd)
{
	char	*num;

	if (!buff || !rd)
		return (1);
	if (rd->fdl == -21)
		dstr_insert_str(buff, "&", buff->strlen);
	else if (rd->fdl >= 0)
	{
		if (!(num = ft_itoa(rd->fdl)))
			return (1);
		dstr_insert_str(buff, num, buff->strlen);
		free(num);
	}
	else
		return (2);
	return (0);
}

int			ilput_rdop(DSTRING *buff, REDIRECT *rd)
{
	int		err;

	err = 0;
	if (!buff || !rd)
		return (1);
	if (rd->type == r_rdr)
		err = dstr_insert_ch(buff, '<', buff->strlen) >= 0 ? 0 : 1;
	else if (!err && rd->type == w_rdr)
		err = dstr_insert_ch(buff, '>', buff->strlen) >= 0 ? 0 : 1;
	else if (!err && rd->type == a_rdr)
		err = dstr_insert_str(buff, ">>", buff->strlen) >= 0 ? 0 : 1;
	else if (!err && rd->type == rw_rdr)
		err = dstr_insert_str(buff, "<>", buff->strlen) >= 0 ? 0 : 1;
	else
		err = !err ? 2 : err;
	if (!err && rd->file == NULL && rd->type != rw_rdr)
		dstr_insert_ch(buff, '&', buff->strlen);
	else
		err = !err ? 2 : err;
	return (err);
}

int			ilput_fdr(DSTRING *buff, REDIRECT *rd)
{
	char	*fdr;

	if (!buff || !rd)
		return (1);
	if (rd->fdr == -42)
		dstr_insert_str(buff, "&-", buff->strlen);
	else if (rd->file)
		dstr_insert_str(buff, rd->file, buff->strlen);
	else if (rd->fdr >= 0)
	{
		if (!(fdr = ft_itoa(rd->fdr)))
			return (1);
		dstr_insert_str(buff, fdr, buff->strlen);
		free(fdr);
	}
	else
		return (2);
	return (0);
}

DSTRING		*ilget_herd(char *s, DSTRING *buff)
{
	dstr_insert_str(buff, "<< ", buff->strlen);
	dstr_insert_str(buff, s, buff->strlen);
	return (buff);
}

DSTRING		*ilget_rd(REDIRECT *rd)
{
	DSTRING		*buff;
	int			err;

	buff = dstr_new(" ");
	err = 0;
	if (!buff)
		sys_fatal_memerr("ILGET_REDIR");
	if (rd->type == herd)
		return (ilget_herd(rd->file, buff));
	err = ilput_fdl(buff, rd);
	!err ? ilput_rdop(buff, rd) : 0;
	!err ? ilput_fdr(buff, rd) : 0;
	if (err)
		dstr_del(&buff);
	return (buff);
}

char		*ilget_rdrs(t_dlist *redirs)
{
	DSTRING		*buff;
	DSTRING		*val;
	char		*rdrs_str;

	if (!(buff = dstr_new("")))
		sys_fatal_memerr("ILGET_REDIR");
	while (redirs)
	{
		val = ilget_rd(redirs->content);
		if (!val)
			sys_fatal_memerr("ILGET_REDIR");
		dstr_insert_dstr(buff, val, buff->strlen);
		dstr_del(&val);
		redirs = redirs->next;
	}
	rdrs_str = buff->txt;
	buff->txt = 0;
	dstr_del(&buff);
	return (rdrs_str);
}

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

	if (!(res = str_from_n_tkls(bkgr->tls, TOKSTR_LMT, 0, TK_ARSHLOCH)))
		if (!(res = ft_strdup(TOKSTR_FAIL)))
			sys_fatal_memerr("ILGET_BKGR");
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
