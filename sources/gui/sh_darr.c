/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_darr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:43:13 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 20:37:19 by gdaemoni         ###   ########.fr       */
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
	if (!darr->count || darr->count == 1)
		return (-1);
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

t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str)
{
	t_darr		rez;
	size_t		i[2];
	DSTRING		*cmp;

	i[1] = -1;
	ft_bzero(&rez, sizeof(rez));
	cmp = dstr_serr(str, dstrrchr(str, '/') + 1, str->strlen);
	i[0] = -1;
	while (++i[0] < darr.count)
		if (ft_strncmp(darr.strings[i[0]]->txt, cmp->txt, cmp->strlen) == 0)
		{
			rez.strings[++i[1]] = dstr_nerr(darr.strings[i[0]]->txt);
			if (++rez.count && rez.maxlen < (size_t)darr.strings[i[0]]->strlen)
				rez.maxlen = darr.strings[i[0]]->strlen;
			rez.allsize += rez.maxlen;
		}
	dstr_del(&cmp);
	return (rez);
}
