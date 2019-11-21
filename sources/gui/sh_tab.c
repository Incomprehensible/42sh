/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:51:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 15:57:58 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"

int				insert_space(DSTRING **buf, t_indch indch)
{
	if (!sh_isdir(*buf, indch.ind_inp))
		dstr_insert_ch(*buf, ' ', (*buf)->strlen);
	return (1);
}

int				move_carret(int ind_inp, int ind, t_buf buffer)
{
	if (buffer.dir && buffer.val)
		return (ind_inp + buffer.dir->strlen + buffer.val->strlen);
	else if (buffer.val)
		return (ind_inp + buffer.val->strlen);
	return (ind);
}

void			sh_tab(DSTRING **buf, t_indch *indch, ENV *env)
{
	t_name_ind		n_ind;
	t_darr			overlap;
	t_buf			buffer;

	ft_bzero(&n_ind, sizeof(t_name_ind));
	n_ind.indch = (*indch);
	n_ind.ind_name = indch->ind_inp;
	buffer = slicer(buf, indch->ind_inp, indch->ind_slash);
	overlap = get_overlap(&buffer, indch, env);
	if (overlap.count == (size_t)-1 && overlap.count)
		unite_buf(buf, &buffer);
	sort_darr(&overlap);
	while (1)
	{
		n_ind = tab_loop(overlap, &buffer, n_ind.fl++, n_ind);
		if (overlap.count <= 1 && insert_space(buf, (*indch)))
			break ;
		indch->ch = ft_getch();
		if (indch->ch != TAB && (indch->fl = 1))
			break ;
	}
	indch->ind = move_carret(indch->ind_inp, indch->ind, buffer);
	unite_buf(buf, &buffer);
	free_t_darr(&overlap);
}
