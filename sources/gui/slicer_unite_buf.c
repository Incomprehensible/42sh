/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slicer_unite_buf.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:21:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/20 21:09:26 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

void			put_var_sls(t_buf *buffer, t_indch *indch, ENV *env)
{
	DSTRING		*val;
	DSTRING		*dir;
	int			ex;

	if (!(dir = dstr_slice(buffer->dir, 0, -1)))
		sys_fatal_memerr("SLICER_RDL_ALLOCA");
	if (!(val = env_get_variable(dir->txt, env)))
		sys_fatal_memerr("SLICER_RDL_ALLOCA");
	dstr_del(&dir);
	ex = val->strlen == 0 ? -1 : sys_is_ex_bin(val->txt);
	if (ex == -2)
	{
		dstr_del(&buffer->dir);
		dstr_slice_del(&buffer->begin, -1, MAX_LL);
		val->txt[val->strlen - 1] != '/' ? \
			dstr_insert_ch(val, '/', val->strlen) : 0;
		buffer->dir = val;
		indch->ind_slash = buffer->begin->strlen + val->strlen;
	}
	else
		dstr_del(&val);
}

t_buf			slicer(DSTRING **buf, t_indch *indch, ENV *env)
{
	t_buf		new;

	new.buf = dstr_nerr((*buf)->txt);
	new.sub = NULL;
	new.dir = NULL;
	new.begin = dstr_serr((*buf), 0, indch->ind_inp);
	if (!indch->ind_slash)
		new.sub = dstr_scerr(buf, indch->ind_inp, \
		end_cut((*buf)->txt, indch->ind_inp, ' '));
	else
	{
		new.sub = dstr_scerr(buf, indch->ind_slash, \
		end_cut((*buf)->txt, indch->ind_inp, ' '));
		new.dir = dstr_scerr(buf, indch->ind_inp, indch->ind_slash);
		if (indch->type_inp == 2 && (indch->type_inp = 1))
			put_var_sls(&new, indch, env);
		
	}
	new.end = dstr_serr((*buf), indch->ind_inp, (*buf)->strlen);
	new.val = NULL;
	new.cut = indch->ind_inp;
	new.slash = indch->ind_slash;
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
