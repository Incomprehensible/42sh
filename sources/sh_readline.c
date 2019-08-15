/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/15 22:22:17 by gdaemoni         ###   ########.fr       */
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

DSTRING			*dstr_new_fast(const char *txt, ssize_t strlen, ssize_t bks)
{
	DSTRING		*dstr;
	
	if (!(dstr = ft_memalloc(sizeof(DSTRING))))
		return (0);
	if (!txt)
		return (dstr);
	dstr->strlen = strlen;
	dstr->bks = bks;
	if (!(dstr->txt = ft_memalloc(dstr->bks * DSTR_BLK_SZ)))
	{
		free(dstr);
		return (0);
	}
	ft_memcpy(dstr->txt, txt, (size_t)dstr->strlen);
	return (dstr);
}

t_darr			sh_cmp_darr(const t_darr darr, const DSTRING *str)
{
	t_darr		rez;
	size_t		i;
	size_t		c;

	i = -1;
	c = -1;
	rez.count = 0;
	rez.allsize = 0;
	rez.maxlen = 0;
	while (++i < darr.count)
	{
		if (ft_strncmp(darr.strings[i]->txt, str->txt, str->strlen) == 0)
		{
			rez.strings[++c] = dstr_new_fast(darr.strings[i]->txt, \
					darr.strings[i]->strlen, darr.strings[i]->bks);
			rez.count++;
			if (rez.maxlen < darr.strings[i]->strlen)
				rez.maxlen = darr.strings[i]->strlen;
			rez.allsize += rez.maxlen;
		}
	}
	return (rez);
}

char			ft_isdir(const char *path)
{
	DIR			*dir;
	char		*dir_name;

	dir_name = ft_concat(2, "00", "./", path);
	dir = opendir(dir_name);
	if (!dir)
	{
		free(dir_name);
		return (0);
	}
	free(dir_name);
	closedir(dir);
	return (1);
}

t_darr			sh_add_path(DSTRING *buf, size_t index, size_t fl)
{
	DSTRING		*str;
	t_darr		darr;
	t_darr		rez;
	
	str = dstr_slice(buf, index, buf->strlen);
	if (ft_isdir(str->txt))
		darr = 	sh_dir_content(str->txt);
	else
		darr = 	sh_dir_content(".");		
	if (str->strlen == 0)
		return (darr);
	else
		rez = sh_cmp_darr(darr, str);
		free_darr(darr.strings);
		dstr_del(&str);
		return (rez);
}

// 1. запомнить позицию курсора
// 2. напечатать таблицу
// 3. вернуть курсор на мето
// 4. при повторном нажатии дописать путь или комнду
size_t			put_col(t_darr darr, size_t n)
{
	return (0);
}

size_t			sh_tab(DSTRING *buf, size_t index)
{
	ssize_t		i;
	t_darr		darr;
	
	if ((i = sh_dstr_iscmd(buf)) == -1)
		ft_putendl("\nGET CMD");
	else
		darr = sh_add_path(buf, i, 1);
	printf("\n%zu\n", darr.count);
	free_darr(darr.strings);
	return (index);
}

char			*sh_readline(void)
{
	DSTRING	*buf;
	
	char	a[2];
	size_t	index;
	char	*move;
	char	fl;

	buf = dstr_new(0);
	index = 0;
	fl = 0;
	while (1)
	{
		a[0] = ft_getch();
		// printf("0x%hhX == [%c]\n", a[0], (a[0] < ' ' || a[0] > '~') ? '$' : a[0]);
		if (a[0] == '\n')
		{
			char *rez = ft_strdup(buf->txt);
			dstr_del(&buf);
			return (buf->txt);
		}
		else if (a[0] == BAKSP || a[0] == DEL)
			index = sh_del_char(&buf, index, a[0]);
		else if (ft_isprint(a[0]) && a[0] != '\t')
			dstr_insert_str(buf, a, index++);
		else if (a[0] == ESC)
			index = sh_esc(index, buf->strlen);
		if (a[0] == TAB && ++fl)
			index = sh_tab(buf, index);
		sh_rewrait(buf, index);
	}
	return (NULL);
}
