/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:43:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 07:45:41 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

void			subst_name(DSTRING **buf, t_darr overlap, int ind, int ind_nam)
{
	dstr_cutins_dstr(buf, overlap.strings[ind], ind_nam);
	sh_rewrite((*buf), (*buf)->strlen);
}

int				ind_begin_cmd(DSTRING *buf)
{
	int			ind;

	if ((ind = dstrrchr(buf, ';')) != -1)
	{
		while (ft_isspace(buf->txt[ind + 1]))
			++ind;
		return (ind + 1);
	}
	ind = 0;
	while (ft_isspace(buf->txt[ind]))
		++ind;
	return (ind);
}

char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir)
{
	DSTRING		*tmp;

	if (start_dir == (*buf)->strlen)
		dstr_insert_ch((*buf), '.', start_dir);
	if ((*buf)->txt[(*buf)->strlen - 1] == '\\')
	{
		tmp = dstr_str_swpc((*buf), "/", (*buf)->strlen - 1);
		if (sh_isdir(tmp, start_dir))
			dstr_cutins_str(buf, "/", (*buf)->strlen - 1);
		dstr_del(&tmp);
		return (1);
	}
	if (sh_isdir((*buf), start_dir) && (*buf)->txt[(*buf)->strlen - 1] != '/')
	{
		dstr_insert_str((*buf), "/", (*buf)->strlen);
		return (1);
	}
	return (1);
}
