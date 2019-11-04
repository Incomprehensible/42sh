/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histrory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:05:05 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/16 18:56:51 by gdaemoni         ###   ########.fr       */
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
