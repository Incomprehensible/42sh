/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histrory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:05:05 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/22 19:32:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"
#include <fcntl.h>
#include <unistd.h>

char			get_histr(t_darr *histr)
{
	int			fd;
	int			ind;
	DSTRING		*line;

	if ((fd = open(HISTORY_PATH, O_CREAT | O_EXCL, S_IREAD | S_IWRITE)) != -1)
		close(fd);
	if ((fd = open(HISTORY_PATH, O_RDONLY)) == -1)
	{
		perror("\nopen failed on get_histr");
		exit(1);
	}
	ind = S_DARR_STRINGS;
	while (get_next_line(fd, &line) == 1 && ind > 0)
	{
		histr->strings[--ind] = line;
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

void			rewrite_histr(t_darr *histr)
{
	int			fd;
	int			ind;
	size_t		count;

	if (!histr->count)
		return ;
	if ((fd = open(HISTORY_PATH, O_RDWR | O_APPEND | O_TRUNC)) == -1)
	{
		ft_putstr("\nopen failed on history command file");
		exit(1);
	}
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

void			clear_history(t_darr *his)
{
	int			fd;

	if ((fd = open(HISTORY_PATH, O_TRUNC)) == -1)
	{
		ft_putstr("\nopen failed on history command file");
		exit(1);
	}
	close(fd);
	free_darr_n(his->strings, his->count);
	his->count = 0;
}

static void		write_cmd_to_buf(int ind, t_darr histr, DSTRING **buf)
{
	if ((size_t)ind == S_DARR_STRINGS - (histr.count + 1))
	{
		dstr_del(buf);
		(*buf) = dstr_new("");
		return ;
	}
	dstr_del(buf);
	(*buf) = dstr_new(histr.strings[ind]->txt);
}

t_indch			show_history(DSTRING **buf, t_indch indc, ENV *envr)
{
	t_darr		his;

	ft_bzero(&his, sizeof(t_darr));
	if (get_histr(&his) == 0)
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
	rewrite_histr(&his);
	return (indc);
}
