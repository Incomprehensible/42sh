/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:43:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/15 17:40:15 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

void			subst_name(DSTRING **buf, t_darr overlap,\
				 int ind, t_name_ind n_ind)
{
	dstr_cutins_dstr(buf, overlap.strings[ind], n_ind.ind_name);
	sh_new_rewrite(n_ind.indch.prompt, (*buf), (*buf)->strlen);
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
	while (buf->txt && ft_isspace(buf->txt[ind]))
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

int				sh_tab_loop_help(t_darr overlap, DSTRING **buf, \
					int fl, t_name_ind n_ind)
{
	if (fl == 0 && overlap.count > 1)
		put_col(overlap, (*buf), n_ind.indch);
	if (overlap.count > 1 && fl != 0)
		subst_name(buf, overlap, n_ind.ind++, n_ind);
	if ((size_t)n_ind.ind == overlap.count)
		n_ind.ind = 0;
	return (n_ind.ind);
}

t_darr			sh_tab_help(DSTRING **buf, ENV *env, t_indch indch)
{
	ssize_t		start_dir;
	t_darr		overlap;

	overlap.count = 0;
	if ((start_dir = sh_dstr_iscmd((*buf))) == -1)
		overlap = sh_add_cmd(buf, env);
	else if (sh_check_back_slash(buf, start_dir))
		overlap = sh_add_path(buf, start_dir);
	sh_new_rewrite(indch.prompt, (*buf), (*buf)->strlen);
	return (overlap);
}
