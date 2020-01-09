/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_hash_actions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 20:59:04 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 20:59:07 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn_hash.h"
#include "hstb.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "libftprintf.h"

int				hsblt_printhashes(t_bltn_ohs *os, ENV *env)
{
	t_hentr		*entry;
	char		*key;
	t_i64		i;
	int			h;
	t_ht_id		htid;

	if (!(htid = env->htid) || !ht_count(htid))
		return (sys_perror("hash: Hash-table is empty", 0, env));
	if (os->args && os->args[0])
	{
		i = 0;
		h = 0;
		while ((key = os->args[i]))
		{
			if ((entry = ht_get(htid, key)))
				hsblt_put_entry(key, entry, h++);
			else
				hprs_no_entry_err(key, "No such entry");
			++i;
		}
	}
	else
		hsblt_put_all(htid);
	return (0);
}

int				hsblt_put_entry(char *key, t_hentr *entry, int frst)
{
	if (!frst)
		ft_printf("%5s  %-18s\n", "hits", "adress :: key");
	ft_printf("%5zu  %s :: %s\n", entry->hits, entry->path, key);
	return (0);
}

int				hsblt_put_all(t_ht_id *htid)
{
	t_i64		i;
	t_hentr		*entry;
	int			h;
	t_htb_hid	*table;
	t_kv		*kv;

	table = ht_b_isid(htid);
	if (!table)
		return (0);
	h = 0;
	i = -1;
	while (++i < (t_i64)table->heap_size && (kv = &(table->heap[i])))
	{
		if (kv->key == NULL)
			continue ;
		entry = (t_hentr *)kv->val;
		hsblt_put_entry(kv->key, entry, h++);
	}
	return (0);
}

int				hprs_no_entry_err(char *entry, char *message)
{
	t_dyn_string	*buff;

	buff = dstr_new("");
	if (!buff)
		sys_fatal_memerr("Hash fail: Allocation error at errhadnling");
	dstr_insert_str(buff, "hash: ", buff->strlen);
	dstr_insert_str(buff, entry, buff->strlen);
	dstr_insert_str(buff, ": ", buff->strlen);
	dstr_insert_str(buff, message, buff->strlen);
	sys_perror(buff->txt, 0, 0);
	dstr_del(&buff);
	return (0);
}

int				hsprs_errhandler(char *mess, int ret_code)
{
	t_dyn_string	*buff;

	buff = dstr_new("");
	if (!buff)
		sys_fatal_memerr("Hash fail: Allocation error at errhadnling");
	dstr_insert_str(buff, "hash: ", buff->strlen);
	dstr_insert_str(buff, mess, buff->strlen);
	sys_perror(buff->txt, 0, 0);
	dstr_del(&buff);
	return (ret_code);
}
