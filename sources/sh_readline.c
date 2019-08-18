/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/18 20:33:24 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include <dirent.h>

// справочники
// http://www.codenet.ru/progr/cpp/7/5.php
// http://rus-linux.net/MyLDP/BOOKS/lpg-04/node8.htm

char			*sh_readline(t_envp *env)
{
	DSTRING	*buf;
	t_indch	indch;
	char	*move;
	char	fl;

	buf = dstr_new(0);
	indch.ind = 0;
	fl = 0;
	while (1)
	{
		if (fl == 0)
			indch.ch = ft_getch();
		// printf("0x%hhX == [%c]\n", a[0], (a[0] < ' ' || a[0] > '~') ? '$' : a[0]);
		if (indch.ch == '\n')
			break ;
		else if ((indch.ch == BAKSP || indch.ch == DEL) && (fl = 0) == 0)
			indch.ind = sh_del_char(&buf, indch.ind, indch.ch);
		else if ((ft_isprint(indch.ch) && indch.ch != '\t') && (fl = 0) == 0)
			dstr_insert_ch(buf, indch.ch, indch.ind++);
		else if ((indch.ch == ESC) && (fl = 0) == 0)
			indch.ind = sh_esc(indch.ind, buf->strlen);
		if (indch.ch == TAB && fl++ == 0)
			indch = sh_tab(&buf, indch.ind, env);
		sh_rewrite(buf, indch.ind);
	}
	char *rez = ft_strdup(buf->txt);
	dstr_del(&buf);
	return (rez);
}
