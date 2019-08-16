/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/16 21:53:15 by gdaemoni         ###   ########.fr       */
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

t_darr			sh_cmp_darr(const t_darr darr, DSTRING *str)
{
	t_darr		rez;
	size_t		i[2];
	DSTRING		*cmp;

	i[1] = -1;
	ft_bzero(&rez, sizeof(rez));
	cmp = dstr_slice(str, dstrrchr(str, '/') + 1, str->strlen);
	if ((i[0] = -1) == -1 && !cmp->strlen)
	{
		dstr_del(&cmp);
		return (darr);
	}
	while (++i[0] < darr.count)
		if (ft_strncmp(darr.strings[i[0]]->txt, cmp->txt, cmp->strlen) == 0)
		{
			rez.strings[++i[1]] = dstr_new_fast(darr.strings[i[0]]->txt, \
					darr.strings[i[0]]->strlen, darr.strings[i[0]]->bks);
			if (++rez.count && rez.maxlen < darr.strings[i[0]]->strlen)
				rez.maxlen = darr.strings[i[0]]->strlen;
			rez.allsize += rez.maxlen;
		}
	dstr_del(&cmp);
	return (rez);
}

char			sh_isdir(DSTRING *buf, ssize_t start_dir)
{
	DIR			*dir;
	DSTRING		*tmp;
	
	tmp = dstr_slice(buf, start_dir, buf->strlen);
	if (tmp->txt[0] != '.' && tmp->txt[0] != '/')
	{
		if (ft_strncmp(tmp->txt, "./", 2))
			dstr_insert_str(tmp, "./", 0);
	}
	dir = opendir(tmp->txt);
	if (!dir)
	{
		dstr_del(&tmp);
		return (0);
	}
	dstr_del(&tmp);
	closedir(dir);
	return (1);
}

char            sh_get_path_help(char *str)
{
   	DIR			*dir;

    dir = opendir(str);
    if (!dir)
        return (0);
    closedir(dir);
    return (1);
}

DSTRING			*sh_get_path(DSTRING *buf, size_t start_dir)
{
	DSTRING		*path1;
	DSTRING		*path2;
	DIR			*dir;
	
	path1 = dstr_slice(buf, start_dir, buf->strlen);
    if (sh_get_path_help(path1->txt))
        return (path1);	
	dstr_insert_str(path1, "./", 0);
	path2 = dstr_slice(path1, 0, dstrrchr(path1, '/'));
    if (sh_get_path_help(path1->txt))
    {
        dstr_del(&path2);
        return (path1);
    }
    if (sh_get_path_help(path2->txt))
    {
        dstr_del(&path1);
        return (path2);
    }
	return (dstr_new("."));
}

t_darr			sh_add_path(DSTRING *buf, size_t start_dir, size_t fl)
{
	DSTRING		*str;
	DSTRING		*path;
	t_darr		darr;
	t_darr		rez;
	
	path = sh_get_path(buf, start_dir);
	darr = sh_dir_content(path->txt);
	
	str = dstr_slice(buf, start_dir, buf->strlen);
	rez = sh_cmp_darr(darr, str);
	free_darr(darr.strings);
	dstr_del(&str);
	dstr_del(&path);
	return (rez);
}

// 1. запомнить позицию курсора
// 2. изменить буфер
// 3. напечатать таблицу еси надо
// 4. вернуть курсор на мето
// 5. при повторном нажатии дописать путь или комнду

void			write_name_buf(t_darr darr, DSTRING **buf, size_t start_dir)
{
	size_t		liter;
	size_t		start_name;
	t_darr		darrcopy;
	DSTRING		*name_file;
	DSTRING		*name_dir;

	name_dir = dstr_slice((*buf), start_dir, (*buf)->strlen);
	start_name = dstrrchr(name_dir, '/');
	name_file = dstr_slice(name_dir, ++start_name, name_dir->strlen);
	darrcopy = darr;
	while (1)
	{
		name_file = dstr_insert_ccut(name_file, darr.strings[0]->txt[name_file->strlen], name_file->strlen);
		darrcopy = sh_cmp_darr(darr, name_file);
		if (darrcopy.count < darr.count)
		{
			name_file->txt[--name_file->strlen] = '\0';
			break ;
		}
		if (ft_strcmp(name_file->txt, darr.strings[0]->txt) == 0)
			break ;
	}
	name_dir = dstr_insert_cut(name_dir, name_file, start_name);
	if (sh_isdir(name_dir, 0))
        dstr_insert_str(name_dir, "/", name_dir->strlen + 1);
	(*buf) = dstr_insert_cut((*buf), name_dir, start_dir);
}

size_t			sh_tab(DSTRING **buf, size_t index)
{
	ssize_t		start_dir;
	t_darr		darr;
	
	if ((start_dir = sh_dstr_iscmd((*buf))) == -1)
		ft_putendl("\nGET CMD");
	else
		darr = sh_add_path((*buf), start_dir, 1);
	write_name_buf(darr, buf, start_dir);
	// printf("\n%zu\n", darr.count);
	// free_darr(darr.strings);
	return ((*buf)->strlen);
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
			break ;
		else if (a[0] == BAKSP || a[0] == DEL)
			index = sh_del_char(&buf, index, a[0]);
		else if (ft_isprint(a[0]) && a[0] != '\t')
			dstr_insert_str(buf, a, index++);
		else if (a[0] == ESC)
			index = sh_esc(index, buf->strlen);
		if (a[0] == TAB && ++fl)
			index = sh_tab(&buf, index);
		sh_rewrait(buf, index);
	}
	char *rez = ft_strdup(buf->txt);
	dstr_del(&buf);
	return (rez);
}
