/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_darr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:43:13 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 19:35:48 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

DSTRING			*dstr_new_fast(const char *txt, ssize_t strlen, ssize_t bks)
{
	DSTRING		*dstr;
	
	if (!(dstr = ft_memalloc(sizeof(DSTRING))))
		return (0);
	if (!txt)
		return (dstr);
	dstr->strlen = strlen;
	dstr->bks = bks;
	if (!(dstr->txt = ft_memalloc(dstr->bks * DSTR_BLK_SZ)))
	{
		free(dstr);
		return (0);
	}
	ft_memcpy(dstr->txt, txt, (size_t)dstr->strlen);
	return (dstr);
}

t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str)
{
	t_darr		rez;
	size_t		i[2];
	DSTRING		*cmp;

	i[1] = -1;
	ft_bzero(&rez, sizeof(rez));
	cmp = dstr_slice(str, dstrrchr(str, '/') + 1, str->strlen);
	if ((i[0] = -1) == -1 && !cmp->strlen)
	{
		dstr_del(&cmp);
		return (darr);
	}
	while (++i[0] < darr.count)
		if (ft_strncmp(darr.strings[i[0]]->txt, cmp->txt, cmp->strlen) == 0)
		{
			rez.strings[++i[1]] = dstr_new((darr.strings[i[0]]->txt));
			if (++rez.count && rez.maxlen < darr.strings[i[0]]->strlen)
				rez.maxlen = darr.strings[i[0]]->strlen;
			rez.allsize += rez.maxlen;
		}
	dstr_del(&cmp);
	return (rez);
}

void			correct_namedir_buf(t_darr darr, DSTRING **buf, size_t start_dir)
{
	size_t		liter;
	size_t		start_name;
	t_darr		darrcopy;
	DSTRING		*name_file;
	DSTRING		*name_dir;

	name_dir = dstr_slice((*buf), start_dir, (*buf)->strlen);
	start_name = dstrrchr(name_dir, '/');
	name_file = dstr_slice(name_dir, ++start_name, name_dir->strlen);
	darrcopy = darr;
	while (1)
	{
		dstr_cutins_ch(&name_file, darr.strings[0]->txt[name_file->strlen], name_file->strlen);
		darrcopy = sh_cmp_darr(darr, name_file);
		if (darrcopy.count < darr.count)
		{
			name_file->txt[--name_file->strlen] = '\0';
			break ;
		}
		if (ft_strcmp(name_file->txt, darr.strings[0]->txt) == 0)
			break ;
		free_darr(darrcopy.strings);
	}
	dstr_cutins_dstr(&name_dir, name_file, start_name);
	dstr_cutins_dstr(buf, name_dir, start_dir);
	dstr_del(&name_dir);
	dstr_del(&name_file);
}
