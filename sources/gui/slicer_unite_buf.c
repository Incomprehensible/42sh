/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slicer_unite_buf.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:21:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 16:09:38 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

t_buf			slicer(DSTRING **buf, int cut, int slash)
{
	t_buf		new;

	new.buf = dstr_nerr((*buf)->txt);
	new.sub = NULL;
	new.dir = NULL;
	if (!slash)
		new.sub = dstr_scerr(buf, cut, end_cut((*buf)->txt, cut, ' '));
	else
	{
		new.sub = dstr_scerr(buf, slash, end_cut((*buf)->txt, cut, ' '));
		new.dir = dstr_scerr(buf, cut, slash);
	}
	new.begin = dstr_serr((*buf), 0, cut);
	new.end = dstr_serr((*buf), cut, (*buf)->strlen);
	new.val = NULL;
	new.cut = cut;
	new.slash = slash;
	return (new);
}

void			unite_buf(DSTRING **buf, t_buf *buffer)
{
	dstr_del(buf);
	if (buffer->val)
	{
		(*buf) = dstr_nerr("");
		dstr_insert_dstr((*buf), buffer->begin, 0);
		if (buffer->dir)
			dstr_insert_dstr((*buf), buffer->dir, (*buf)->strlen);
		dstr_insert_dstr((*buf), buffer->val, (*buf)->strlen);
		dstr_insert_dstr((*buf), buffer->end, (*buf)->strlen);
	}
	else
		(*buf) = dstr_nerr(buffer->buf->txt);
	dstr_del(&(buffer->buf));
	dstr_del(&(buffer->begin));
	dstr_del(&(buffer->end));
	dstr_del(&(buffer->sub));
	if (buffer->dir)
		dstr_del(&(buffer->dir));
}
