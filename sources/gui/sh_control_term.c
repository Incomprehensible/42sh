/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_control_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:35:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/04 21:55:33 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

struct termios	g_std_term;

void			put_col_his(t_darr his, char fl, const DSTRING *buf)
{
	int			ind;
	int			count;
	t_darr		col;

	ind = S_DARR_STRINGS - his.count;
	count = -1;
	while (++count < (int)his.count)
		col.strings[count] = dstr_nerr(his.strings[ind++]->txt);
	col.allsize = his.allsize;
	col.maxlen = his.maxlen;
	col.count = his.count;
	if (fl)
		sort_darr(&col);
	//put_col(col, buf);
	free_darr_n(col.strings, col.count);
}

char			ispers_arws(char ch, t_indch *indch, \
					t_darr *his, const DSTRING *buf)
{
	if (ch == 0xB)
		put_col_his(*his, 0, buf);
	else if (ch == 0x10)
		put_col_his(*his, 1, buf);
	else if (ch == ESC)
	{
		ch = ft_getch();
		if (ch == '[')
		{
			ch = ft_getch();
			indch->fl = 1;
			return (ch);
		}
	}
	indch->fl = 2;
	return (ch);
}

t_indch			sh_esc(t_indch indch, const size_t max, DSTRING **buf, \
					ENV *envr)
{
	if (!indch.fl)
		indch.ch = ft_getch();
	if (indch.ch == '[' || indch.fl)
	{
		if (!indch.fl)
			indch.ch = ft_getch();
		indch.fl = 0;
		if (indch.ch == LEFT[0] && indch.ind > 0)
		{
			indch.ind--;
			return (indch);
		}
		else if (indch.ch == RIGHT[0] && (size_t)indch.ind < max)
		{
			indch.ind++;
			return (indch);
		}
		else if (indch.ch == UP[0] || indch.ch == DOWN[0])
			return (show_new_history(buf, indch, envr));
		else if (indch.ch == 51)
			sh_del_char(buf, indch.ind, ft_getch());
	}
	return (indch);
}

int				ft_getch(void)
{
	char			ch;

	ch = 0;
	if ((read(0, &ch, 1)) <= 0)
		ch = (char)0xff;
	return ((int)ch);
}

void			sys_term_init(void)
{
	tcgetattr(0, &g_std_term);
}

void			sys_term_restore(void)
{
	tcsetattr(0, TCSANOW, &g_std_term);
}

void			sys_term_noecho(void)
{
	struct termios	newt;

	newt = g_std_term;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &newt);
}
