/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histrory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:05:05 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/22 20:57:09 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"
#include <fcntl.h>
#include <unistd.h>

int				get_history_fd(int flags, char *er_context, ENV *envr)
{
	int			fd;
	char		*hist_pt;

	hist_pt = sys_get_conf_path(SH_HIST_FILE, envr);
	if ((fd = sys_file_op(hist_pt, envr, flags, er_context)) < 0)
	{
		free(hist_pt);
		return (-1);
	}
	free(hist_pt);
	return (fd);
}

char			get_histr(t_darr *histr, ENV *envr)
{
	int			fd;
	int			ind;
	DSTRING		*line;

	if ((fd = get_history_fd(O_RDONLY, "GET_HISTORY: File error", envr)) < 0)
		return (0);
	ind = S_DARR_STRINGS;
	while (get_next_line(fd, &line) == 1 && ind > 0)
	{
		histr->strings[--ind] = check_null(line);
		histr->allsize += histr->strings[ind]->strlen;
		if (++histr->count && (size_t)histr->strings[ind]->strlen > histr->maxlen)
			histr->maxlen = histr->strings[ind]->strlen;
	}
	dstr_del(&line);
	close(fd);
	if (histr->count)
		return (1);
	return (0);
}

void			rewrite_histr(t_darr *histr, ENV *envr)
{
	int			fd;
	int			ind;
	size_t		count;

	if (!histr->count || ((fd = get_history_fd \
	(O_WRONLY | O_TRUNC, "REWRITE_HISTORY: File error", envr)) < 0))
		return ;
	if (histr->count > 1000)
		ind = S_DARR_STRINGS - (histr->count - 1000);
	else
		ind = S_DARR_STRINGS - 1;
	count = 0;
	while (count <= 1000 && count < histr->count)
	{
		write(fd, histr->strings[ind]->txt, histr->strings[ind]->strlen);
		write(fd, "\n", 1);
		ind--;
		count++;
	}
	close(fd);
	free_darr_re(histr->strings, histr->count);
}

static void		write_cmd_to_buf(int ind, t_darr histr, DSTRING **buf)
{
	if ((size_t)ind == S_DARR_STRINGS - (histr.count + 1))
	{
		dstr_del(buf);
		(*buf) = dstr_nerr("");
		return ;
	}
	dstr_del(buf);
	(*buf) = dstr_nerr(histr.strings[ind]->txt);
}

t_indch			show_history(DSTRING **buf, t_indch indc, ENV *envr)
{
	t_darr		his;

	ft_bzero(&his, sizeof(t_darr));
	if (get_histr(&his, envr) == 0)
		return (indc);
	if (indc.his == 0)
		indc.his = S_DARR_STRINGS - (his.count + 1);
	if (indc.ch == UP[0] && (*buf)->strlen && indc.ind++)
		sys_write_history(*buf, envr);
	while (1)
	{
		if (indc.ch == UP[0] && (indc.his + 1) < S_DARR_STRINGS)
			write_cmd_to_buf(++indc.his, his, buf);
		else if (indc.ch == DOWN[0] \
			&& ((size_t)indc.his - 1) >= S_DARR_STRINGS - (his.count + 1))
			write_cmd_to_buf(--indc.his, his, buf);
		indc.ind = (*buf)->strlen;
		sh_rewrite((*buf), (*buf)->strlen);
		indc.ch = ispers_arws(ft_getch(), &indc, &his, (*buf));
		if (indc.ch != UP[0] && indc.ch != DOWN[0] \
			&& indc.ch != 0xB && indc.ch != 0x10)
			break ;
	}
	rewrite_histr(&his, envr);
	return (indc);
}
