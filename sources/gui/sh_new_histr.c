/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_new_histr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 14:47:35 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/14 20:57:46 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"
#include <fcntl.h>
#include <unistd.h>

int				get_new_history_fd(int flags, char *er_context, ENV *envr)
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

char			get_new_histr(t_darr *histr, ENV *envr)
{
	int			fd;
	int			ind;
	DSTRING		*line;

	if ((fd = get_new_history_fd(O_RDONLY, "GET_HISTORY: File error", envr)) < 0)
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

void		init_histr(ENV *envr)
{
	ft_bzero(&g_histr, sizeof(t_darr));
	if (get_new_histr(&g_histr, envr) == 0) // histrory init error
		return ;
}

char		skip_esc(char ch)
{
	if (ch == 27)
	{
		ch = ft_getch();
		ch = ft_getch();
	}
	return (ch);	
}	

int		write_history_buf(char side, int ind, DSTRING **buf, t_indch indc)
{
	ind = (side == UP[0]) ? ++ind : --ind;
	dstr_del(buf);
	(*buf) = dstr_nerr(g_histr.strings[ind]->txt);
	sh_new_rewrite(indc.prompt, (*buf), (*buf)->strlen);
	return (ind);
}

t_indch		show_new_history(DSTRING **buf, t_indch indc, ENV *envr)
{
	int		ind;

	ind = S_DARR_STRINGS - (g_histr.count + 1);
	g_histr.strings[ind] = dstr_nerr((*buf)->txt);
	while (1)
	{
		if (indc.ch == UP[0] && (ind + 1) < S_DARR_STRINGS)
			ind = write_history_buf(UP[0], ind, buf, indc);
		else if (indc.ch == DOWN[0] && \
			(ind - 1) > (S_DARR_STRINGS - (g_histr.count + 1)))
			ind = write_history_buf(DOWN[0], ind, buf, indc);
		if ((indc.ch = ft_getch()) == ESC) 
		{
			indc.ch = ft_getch();
			if ((indc.ch = ft_getch()) != UP[0] && \
				(indc.ch = ft_getch()) != DOWN[0])
				break ;				
		}
		else
			break ;
	}
	indc.fl = 1;
	indc.ind = (*buf)->strlen;
	return (indc);
}
