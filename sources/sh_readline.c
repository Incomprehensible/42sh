/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 18:49:06 by gdaemoni         ###   ########.fr       */
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
	
	char	a[2];
	size_t	index;
	char	*move;
	char	fl;

	buf = dstr_new(0);
	index = 0;
	while (1)
	{
		a[0] = ft_getch();
		// printf("0x%hhX == [%c]\n", a[0], (a[0] < ' ' || a[0] > '~') ? '$' : a[0]);
		if (a[0] == '\n')
			break ;
		else if (a[0] == BAKSP || a[0] == DEL)
			index = sh_del_char(&buf, index, a[0]);
		else if (ft_isprint(a[0]) && a[0] != '\t')
			dstr_insert_str(buf, a, index++);
		else if (a[0] == ESC)
			index = sh_esc(index, buf->strlen);
		if (a[0] == TAB)
			index = sh_tab(&buf, index, env);
		sh_rewrite(buf, index);
	}
	char *rez = ft_strdup(buf->txt);
	dstr_del(&buf);
	return (rez);
}
