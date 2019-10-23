/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_search_history.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 18:19:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/20 17:09:51 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <fcntl.h>
#include <unistd.h>

char		get_list_histr(t_darr *histr, ENV *envr)
{
	int			fd;
	int			ind;
	DSTRING		*line;

	if ((fd = get_history_fd(O_RDONLY, "GET_HISTORY: file error", envr)) < 0)
		return (0);
	ind = -1;
	while (++ind != -1 && get_next_line(fd, &line) == 1)
	{
		histr->strings[ind] = check_null(line);
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

void		init_param(t_darr *his, DSTRING **strd, DSTRING **strs, ENV *envr)
{
	ft_bzero(his, sizeof(t_darr));
	if (get_list_histr(his, envr) == 0)
		return ;
	(*strd) = dstr_nerr("- search_history : ");
	(*strs) = dstr_nerr("");
}

t_darr		get_overlap(const t_darr his, DSTRING *strs);


char		direction(t_darr his, DSTRING **strd, DSTRING **strs)
{
	t_indch	ich;
	int		c;
	t_darr	o;

	ich.ind = 0;
	c = -1;
	ft_bzero(&o, sizeof(t_darr));
	while (1)
	{
		free_t_darr(&o);
		sh_new_rewrite(dstr_nerr("lol "),(*strd), LENSERCH + c);
		ich.ch = ft_getch();
		if (ft_isprint(ich.ch) && !(ich.ind = 0) && ++c != -1)
			dstr_insert_ch((*strs), ich.ch, (*strs)->strlen);
		if ((*strs)->strlen)
			o = get_overlap(his, (*strs));
		direction_help(o, his, &ich, strd);
		if ((!ft_isprint(ich.ch) && ich.ch != 0x12 && ich.ch != TAB) \
				|| (o.count <= 1 && ich.ch != TAB))
			break ;
	}
	free_t_darr(&o);
	return (ich.ch);
}

t_indch		search_history(DSTRING **buf, ENV *envr)
{
	t_darr		his;
	DSTRING		*strd;
	DSTRING		*strs;
	t_indch		indch;

	init_param(&his, &strd, &strs, envr);
	if (his.count)
	{
		indch.ch = direction(his, &strd, &strs);
		free_t_darr(&his);
		dstr_del(buf);
		(*buf) = dstr_serr(strd, LENSERCH - 1, strd->strlen);
		if ((*buf)->strlen && (indch.fl = 1))
			dstr_insert_ch((*buf), ' ', (*buf)->strlen);
	}
	dstr_del(&strs);
	dstr_del(&strd);
	indch.ind = (*buf)->strlen;
	return (indch);
}
