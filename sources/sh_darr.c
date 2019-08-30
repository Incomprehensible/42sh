/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_darr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:43:13 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:03:53 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

static void		sh_swap(DSTRING **var1, DSTRING **var2)
{
	DSTRING		*tmp;

	tmp = (*var1);
	(*var1) = (*var2);
	(*var2) = tmp;
}

int				sort_darr(t_darr *darr)
{
	size_t		index;
	char		fl;

	fl = 1;
	while (fl--)
	{
		index = -1;
		while (darr->strings[++index + 1])
		{
			if (ft_strcmp(darr->strings[index]->txt, \
						darr->strings[index + 1]->txt) > 0 && (fl = 1))
				sh_swap(&darr->strings[index + 1], &darr->strings[index]);
		}
	}
	return (1);
}

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
	i[0] = -1;
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

void			correct_namedir_buf(t_darr darr, DSTRING **buf, size_t st_dir)
{
	ssize_t		start_name;
	t_darr		darrcopy;
	DSTRING		*n_f;
	DSTRING		*name_dir;

	name_dir = dstr_slice((*buf), st_dir, (*buf)->strlen);
	start_name = dstrrchr(name_dir, '/');
	n_f = dstr_slice(name_dir, ++start_name, name_dir->strlen);
	darrcopy.count = 0;
	while (darr.count)
	{
		dstr_cutins_ch(&n_f, darr.strings[0]->txt[n_f->strlen], n_f->strlen);
		darrcopy = sh_cmp_darr(darr, n_f);
		if (darrcopy.count != darr.count)
		{
			n_f->txt[--n_f->strlen] = '\0';
			break ;
		}
		if (ft_strcmp(n_f->txt, darr.strings[0]->txt) == 0)
			break ;
		free_darr_n(darrcopy.strings, darrcopy.count);
	}
	dstr_cutins_dstr(&name_dir, n_f, start_name);
	dstr_cutins_dstr(buf, name_dir, st_dir);
	help_correct_namedir_buf(&darrcopy, &n_f, &name_dir);
}
