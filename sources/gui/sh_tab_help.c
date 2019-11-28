/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:47:43 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 20:07:35 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

void		check_spec_symbols(DSTRING **str)
{
	int			i;

	i = -1;
	while (++i < (*str)->strlen)
		if (ft_strchr(SPEC_SYMBOLS, (*str)->txt[i]))
			dstr_insert_ch((*str), '\\', i++);
}

static void	subst_val(t_buf *buffer, t_darr overlap, t_name_ind n_ind)
{
	DSTRING		*new;

	new = dstr_nerr("");
	buffer->val = overlap.strings[n_ind.ind];
	dstr_insert_dstr(new, buffer->begin, 0);
	if (buffer->dir)
		dstr_insert_dstr(new, buffer->dir, new->strlen);
	if (!n_ind.road && n_ind.indch.type_inp != 2)
		check_spec_symbols(&overlap.strings[n_ind.ind]);
	dstr_insert_dstr(new, overlap.strings[n_ind.ind], new->strlen);
	dstr_insert_dstr(new, buffer->end, new->strlen);
	sh_rewrite(n_ind.indch.prompt, new, buffer->cut < buffer->slash ? \
	buffer->slash + buffer->val->strlen : buffer->cut + buffer->val->strlen, -1);
	dstr_del(&new);
}

t_name_ind	tab_loop(t_darr overlap, t_buf *buffer, int fl, t_name_ind n_ind)
{
	if (!fl && overlap.count > 1 && overlap.count != (size_t)-1)
		put_col(overlap, buffer->buf, n_ind.indch);
	else if (overlap.count > 1)
	{
		subst_val(buffer, overlap, n_ind);
		n_ind.ind++;
	}
	else if (overlap.count == 1)
		subst_val(buffer, overlap, n_ind);
	if ((size_t)n_ind.ind == overlap.count)
	{
		n_ind.ind = 0;
		n_ind.road = 1;
	}
	return (n_ind);
}
