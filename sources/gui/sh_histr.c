/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 14:47:35 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 20:06:55 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"
#include <fcntl.h>
#include <unistd.h>

char			get_new_histr(t_darr *histr, ENV *envr)
{
	int			fd;
	int			ind;
	DSTRING		*line;

	if ((fd = get_history_fd(O_RDONLY, \
				"GET_HISTORY: File error", envr)) < 0)
		return (0);
	ind = S_DARR_STRINGS;
	while (get_next_line(fd, &line) == 1 && ind > 0)
	{
		histr->strings[--ind] = check_null(line);
		histr->allsize += histr->strings[ind]->strlen;
		if (++histr->count && (size_t)histr->strings[ind]->strlen > \
					histr->maxlen)
			histr->maxlen = histr->strings[ind]->strlen;
	}
	dstr_del(&line);
	close(fd);
	if (histr->count)
		return (1);
	return (0);
}

int				write_history_buf(char side, int ind, DSTRING **buf, \
					t_indch indc)
{
	ind = (side == UP[0]) ? ++ind : --ind;
	dstr_del(buf);
	(*buf) = dstr_nerr(g_histr.strings[ind]->txt);
	sh_rewrite(indc.prompt, (*buf), (*buf)->strlen, -1);
	return (ind);
}

int				get_oldbuf(DSTRING **buf, DSTRING *oldbuf, t_indch indch)
{
	if (ft_strequ((*buf)->txt, oldbuf->txt))
	{
		sh_rewrite(indch.prompt, (*buf), (*buf)->strlen, -1);
		return (S_DARR_STRINGS - g_histr.count - 1);
	}
	dstr_del(buf);
	(*buf) = dstr_nerr(oldbuf->txt);
	sh_rewrite(indch.prompt, (*buf), (*buf)->strlen, -1);
	return (S_DARR_STRINGS - g_histr.count - 1);
}

t_indch			skip_esc(t_indch indch)
{
	char	ch;

	ch = ft_getch();
	indch.fl = 1;
	if (ch == ESC)
	{
		ch = ft_getch();
		ch = ft_getch();
		if (ch != UP[0] && ch != DOWN[0])
			indch.fl = 0;
	}
	indch.ch = ch;
	return (indch);
}

t_indch			show_history(DSTRING **buf, t_indch indc)
{
	size_t		ind;
	DSTRING		*oldbuf;

	ind = S_DARR_STRINGS - g_histr.count - 1;
	oldbuf = dstr_nerr((*buf)->txt);
	while (1)
	{
		if (indc.ch == UP[0] && (ind + 1) < S_DARR_STRINGS)
			ind = write_history_buf(UP[0], ind, buf, indc);
		else if (indc.ch == DOWN[0] && \
			(ind - 1) > (S_DARR_STRINGS - (g_histr.count + 1)))
			ind = write_history_buf(DOWN[0], ind, buf, indc);
		else if ((ind - 1) == (S_DARR_STRINGS - (g_histr.count + 1)))
			ind = get_oldbuf(buf, oldbuf, indc);
		if ((indc = skip_esc(indc)).ch != UP[0] && indc.ch != DOWN[0])
			break ;
	}
	dstr_del(&oldbuf);
	indc.ind = (*buf)->strlen;
	return (indc);
}
