/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_exprs_strings_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 08:18:51 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:19:32 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"

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
