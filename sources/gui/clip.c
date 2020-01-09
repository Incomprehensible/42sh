/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 22:08:26 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 22:41:27 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

void			clip_index(int x1, int x2, int *y1, int *y2)
{
	if (x1 > x2)
	{
		*y1 = x2;
		*y2 = x1;
	}
	else
	{
		*y2 = x2;
		*y1 = x1;
	}
}

void			clip_paste(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	indch->select = 0;
	if (clip->buffer->strlen)
	{
		dstr_insert_dstr(*buf, clip->buffer, indch->ind);
		indch->ind += clip->buffer->strlen;
	}
}

void			clip_cut(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	int			ei;
	int			bi;

	clip_index(clip->ind, indch->ind, &bi, &ei);
	if (clip->buffer)
		dstr_del(&(clip->buffer));
	indch->ind = bi;
	clip->buffer = dstr_scerr(buf, (ssize_t)bi, (ssize_t)ei);
	indch->select = 0;
}

void			clip_copy(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	int			ei;
	int			bi;

	clip_index(clip->ind, indch->ind, &bi, &ei);
	if (clip->buffer)
		dstr_del(&(clip->buffer));
	indch->ind = bi;
	clip->buffer = dstr_serr(*buf, (ssize_t)bi, (ssize_t)ei);
}

void			clip_work(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	if (indch->ch == 0xb && indch->select)
		clip_copy(indch, clip, buf);
	else if (indch->ch == 0x10)
		clip_paste(indch, clip, buf);
	else if (indch->ch == 0x18 && indch->select)
		clip_cut(indch, clip, buf);
}
