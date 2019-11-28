/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_search_his_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:15:19 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 15:17:19 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

void		print_his_line(DSTRING *str_srch, DSTRING *str_over)
{
	DSTRING			*prompt;
	DSTRING			*tmp;

	prompt = dstr_nerr("search in history ");
	if (!str_srch->strlen)
		sh_rewrite(prompt, str_srch, 0);
	else
	{
		tmp = dstr_nerr(str_srch->txt);
		dstr_insert_ch(tmp, '|', 0);
		dstr_insert_str(tmp, "| ", tmp->strlen);
		if (str_over != NULL)
			dstr_insert_str(tmp, str_over->txt, tmp->strlen);
		else
			dstr_insert_str(tmp, "", tmp->strlen);
		sh_rewrite(prompt, tmp, str_srch->strlen);
		dstr_del(&tmp);
	}
	dstr_del(&prompt);
}

void		skip_escp(void)
{
	if (ft_getch() != '[')
		return ;
	ft_getch();
}

void		print_his_help_col(t_darr overlap)
{
	ushort		col;
	int			iter;
	DSTRING		*colstr;

	iter = 0;
	col = get_col(overlap.maxlen + 2);
	ft_putstr("\n");
	ft_putstr(MOVEBGN);
	free_lines_down();
	colstr = sh_get_col(overlap, col, iter);
	ft_putstr(colstr->txt);
	ft_putstr("\n");
	dstr_del(&colstr);
}

void		print_his_col(DSTRING *str_srch)
{
	int					i;
	size_t				c;
	int					j;
	t_darr				rez;
	struct winsize		te;

	i = S_DARR_STRINGS;
	c = (size_t)-1;
	j = 0;
	ft_bzero(&rez, sizeof(t_darr));
	ioctl(0, TIOCGWINSZ, &te);
	while (++c < g_histr.count)
	{
		if (ft_strncmp(str_srch->txt, g_histr.strings[--i]->txt,\
		str_srch->strlen) == 0 && g_histr.strings[i]->strlen < (te.ws_col - 3))
		{
			rez.strings[j++] = g_histr.strings[i];
			if ((ssize_t)rez.maxlen < g_histr.strings[i]->strlen)
				rez.maxlen = g_histr.strings[i]->strlen;
			rez.allsize += g_histr.strings[i]->strlen;
		}
	}
	rez.count = j;
	if (j)
		print_his_help_col(rez.count ? rez : g_histr);
}
