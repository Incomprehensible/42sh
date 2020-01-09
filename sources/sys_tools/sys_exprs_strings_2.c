/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_exprs_strings_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 08:16:14 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:18:34 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_token_to_str.h"
#include "sys_tools/sys_errors.h"

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
