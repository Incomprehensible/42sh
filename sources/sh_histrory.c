/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histrory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:05:05 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:39:37 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <fcntl.h>
#include <unistd.h>

char			get_histr(t_darr *histr)
{
	int			fd;
	int			ind;
	char		*line;

	if ((fd = open(HISTORY_PATH, O_RDONLY)) == -1)
	{
		perror("\nopen failed on get_histr");
		exit(1);
	}
	ind = S_DARR_STRINGS;
	while (get_next_line(fd, &line) == 1 && ind > 0)
	{
		histr->strings[--ind] = dstr_new(line);
		free(line);
		++histr->count;
		histr->allsize += histr->strings[ind]->strlen;
		if (histr->strings[ind]->strlen > histr->maxlen)
			histr->maxlen = histr->strings[ind]->strlen;
	}
	close(fd);
	if (histr->count)
		return (1);
	return (0);
}

void			rewrite_histr(t_darr *histr)
{
	int			fd;
	int			ind;
	int			count;

	if (!histr->count)
		return ;
	if ((fd = open(HISTORY_PATH, O_RDWR | O_APPEND | O_TRUNC)) == -1)
	{
		perror("\nopen failed on history command file");
		exit(1);
	}
	ind = S_DARR_STRINGS - 1;
	count = 0;
	while (count < 1000 && count < histr->count)
	{
		write(fd, histr->strings[ind]->txt, histr->strings[ind]->strlen);
		write(fd, "\n", 1);
		ind--;
		count++;
	}
	free_darr_n(histr->strings, histr->count);
	close(fd);
}

void			clear_history(t_darr *his)
{
	int			fd;

	fd = open(HISTORY_PATH, O_TRUNC);
	close(fd);
	free_darr_n(his->strings, his->count);
	his->count = 0;
}

static void		write_cmd_to_buf(int ind, t_darr histr, DSTRING **buf)
{
	if (ind == S_DARR_STRINGS - (histr.count + 1))
	{
		dstr_del(buf);
		(*buf) = dstr_new("");
		return ;
	}
	dstr_del(buf);
	(*buf) = dstr_new(histr.strings[ind]->txt);
}

t_indch			show_history(DSTRING **buf, t_indch indc)
{
	t_darr		his;

	ft_bzero(&his, sizeof(t_darr));
	if (get_histr(&his) == 0)
		return (indc);
	if (indc.his == 0)
		indc.his = S_DARR_STRINGS - (his.count + 1);
	if (indc.ch == UP[0] && (*buf)->strlen && indc.ind++)
		write_history(*buf);
	while (1)
	{
		if (indc.ch == UP[0] && (indc.his + 1) < S_DARR_STRINGS)
			write_cmd_to_buf(++indc.his, his, buf);
		else if (indc.ch == DOWN[0] \
			&& (indc.his - 1) >= S_DARR_STRINGS - (his.count + 1))
			write_cmd_to_buf(--indc.his, his, buf);
		indc.ind = (*buf)->strlen;
		sh_rewrite((*buf), (*buf)->strlen);
		indc.ch = ispers_arws(ft_getch(), &indc, &his);
		if (indc.ch != UP[0] && indc.ch != DOWN[0] \
			&& indc.ch != 0xB && indc.ch != 0x10)
			break ;
	}
	rewrite_histr(&his);
	return (indc);
}
