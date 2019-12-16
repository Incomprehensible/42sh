/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_setreset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 20:59:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 21:00:02 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn_hash.h"
#include "hstb.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "libftprintf.h"

t_bltn_ohs		hash_parse_args(char **args)
{
	size_t		i;
	int			state;
	t_bltn_ohs	ops;

	state = 1;
	i = 1;
	ft_bzero(&ops, sizeof(t_bltn_ohs));
	while (state != 0)
	{
		if (state == 1)
			state = hprs_args(&ops, &i, args);
		else if (state == 2)
			state = hprs_flags(&ops, &i, args);
		else if (state < 0)
			state = hprs_errors(state, &ops);
	}
	return (ops);
}

int				hsblt_reset(t_bltn_ohs *os, ENV *env)
{
	char		*key;
	t_hentr		*entry;
	t_ht_id		htid;
	size_t		i;

	htid = env->htid;
	i = 0;
	if (os->args && os->args[0])
	{
		while ((key = os->args[i]))
		{
			entry = ht_get(htid, key);
			if (!entry)
				hprs_no_entry_err(os->args[i], "No such entry");
			else
				entry->hits = 0;
			++i;
		}
	}
	else
	{
		ht_free(htid);
		env->htid = ht_create(free, hs_fnv1a_64, CACHE_STD_SIZE);
	}
	return (0);
}

int				hsblt_sethashes(t_bltn_ohs *os, ENV *env)
{
	t_hentr		entry;
	char		*key;
	size_t		i;
	t_ht_id		htid;

	ft_putendl("seths");
	i = 0;
	if (!(htid = env->htid))
		return (0);
	while ((key = os->args[i]))
	{
		entry.hits = 0;
		ft_strncpy(entry.path, os->path, PATH_MAX);
		ht_add(htid, os->args[i], &entry, sizeof(t_hentr));
		++i;
	}
	return (0);
}
