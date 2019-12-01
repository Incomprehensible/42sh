/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_put_col.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:33:07 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 19:18:48 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <sys/ioctl.h>
#include "sys_tools/sys_errors.h"

static void		ft_strset(char **str, char c, size_t n)
{
	size_t		i;

	i = 0;
	(*str) = (char*)malloc(sizeof(char) * n + 1);
	if ((*str) == NULL)
		sys_fatal_memerr(sys_get_std_message(8));
	while (i < n)
		(*str)[i++] = c;
	(*str)[i] = '\0';
}

static DSTRING	*sh_get_space(int max, int len)
{
	char	*str;
	int		n;
	DSTRING	*rez;

	n = max - len;
	if (n > 0)
	{
		ft_strset(&str, ' ', n);
		rez = dstr_nerr(str);
		free(str);
		return (rez);
	}
	return (dstr_nerr(""));
}

DSTRING			*sh_get_col(t_darr dar, const ushort col, ushort iter)
{
	DSTRING		*colstr;
	DSTRING		*space;
	ushort		nstr;
	uint		ind;

	nstr = -1;
	colstr = dstr_nerr("");
	while (++nstr < dar.count / col + 1 && (iter = 0) == 0)
	{
		ind = nstr;
		while (iter++ < col)
		{
			if (ind < dar.count)
			{
				dstr_insert_dstr(colstr, dar.strings[ind], colstr->strlen);
				space = sh_get_space(dar.maxlen + 2, dar.strings[ind]->strlen);
				dstr_insert_dstr(colstr, space, colstr->strlen);
				dstr_del(&space);
			}
			ind += dar.count / col ? dar.count / col + 1 : 1;
		}
		dstr_insert_str(colstr, "\n", colstr->strlen);
	}
	return (colstr);
}

void			free_lines_down(void)
{
	struct winsize	term;
	int				x;
	int				y;

	ioctl(0, TIOCGWINSZ, &term);
	get_coord_caret(&y, &x);
	ft_putstr(SAVECAR);
	while (y++ != term.ws_row)
	{
		ft_putstr(NEXTLIN);
		ft_putstr(CLEARL);
	}
	ft_putstr(LOADCAR);
}

void			put_col(t_darr overlap, const DSTRING *buf, t_indch indch)
{
	ushort		col;
	int			iter;
	DSTRING		*colstr;

	iter = 0;
	col = get_col(overlap.maxlen + 2);
	ft_putstr("\n");
	free_lines_down();
	colstr = sh_get_col(overlap, col, iter);
	dstr_cutins_ch(&colstr, '\0', colstr->strlen - 1);
	ft_putstr(colstr->txt);
	ft_putstr("\n");
	ft_putstr(indch.prompt->txt);
	sh_rewrite(indch.prompt, buf, indch.ind, -1);
	dstr_del(&colstr);
}
