/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/25 09:16:56 by gdaemoni         ###   ########.fr       */
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



DSTRING			*get_regular(DSTRING *buf)
{
	DSTRING		*reg;
	int			i;

	i = 0;
	while (ft_isspace(buf->txt[i]))
		i++;
	while (ft_isalpha(buf->txt[i]) || buf->txt[i] == '*')
		i++;
	while (ft_isspace(buf->txt[i]))
		i++;
	reg = dstr_slice(buf, i, buf->strlen);
	return (reg);
}

t_darr			get_paths(DSTRING *buf)
{
	DSTRING		*reg;
	t_darr		paths;
	char		**splitpath;
	int			i;

	reg = get_regular(buf);
	splitpath = ft_strsplit(reg->txt, '/');
	paths.count = 0;
	paths.allsize = 0;
	paths.maxlen = 0;
	i = -1;
	while (splitpath[++i])
	{
		paths.strings[i] = dstr_new(splitpath[i]);
		paths.count++;
		paths.allsize += paths.strings[i]->strlen;
		if (paths.maxlen < paths.strings[i]->strlen)
			paths.maxlen = paths.strings[i]->strlen;
	}
	return (paths);
}

char			isdir(char *name, DSTRING *path)
{
	DIR			*dir;
	
	dir = opendir(name);
	if (!dir)
	{
		dstr_insert_str(path, "/", path->strlen);
		dstr_insert_str(path, name, path->strlen);
		dir = opendir(path->txt);
		if (!dir)
			return (0);
	}
	closedir(dir);
	return (1);
}

// void			cmp_paths(t_darr *rez, t_darr paths, int i)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	DSTRING 		*path;
// 	DSTRING 		*new;
// 	int				j;

// 	path = rez->strings[i];
// 	dir = opendir(path->txt);
// 	j = i + 1;
// 	if (!dir)
// 		return ;
// 	while ((entry = readdir(dir)))
// 	{
// 		if (!paths.strings[i]->txt)
// 			continue ;
// 		if (nmatch(entry->d_name, paths.strings[i]->txt))
// 		{
// 			if (!rez->strings[j])
// 				rez->strings[j] = dstr_new("");
// 			if (isdir(entry->d_name, path))
// 			{
// 				new = dstr_new(entry->d_name);
// 				dstr_insert_ch(new, '/', new->strlen);
// 				dstr_insert_dstr(rez->strings[j], new, rez->strings[j]->strlen);
// 				j++;
// 			}
// 			else
// 			{
// 				dstr_insert_str(rez->strings[j], entry->d_name, rez->strings[j]->strlen);
// 				j++;
// 			}
// 		}
// 	}
// 	rez->count = j;
// 	closedir(dir);
// }

void			write_buf(DSTRING **buf, t_darr rez)
{
	int			i;
	DSTRING		*dstr;

	i = 0;
	dstr = dstr_new("");
	while (i < rez.count)
	{
		dstr_insert_dstr(dstr, rez.strings[i], dstr->strlen);
		dstr_insert_ch(dstr, ' ', dstr->strlen);
		++i;
	}
	ft_putstr(dstr->txt);
}




// t_darr			cmp_dir_reg(DSTRING *n_dir, DSTRING *reg, DSTRING *next)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	t_darr			rez;
// 	DSTRING			*cmp;
// 	int				i;

// 	dir = opendir(n_dir->txt);
// 	if (!dir)
// 		ft_putstr("/nNOT DIR\n");
// 	i = -1;
// 	while ((entry = readdir(dir)))
// 	{
// 		cmp = add_aster(entry->d_name);
// 		if (is_strdot(entry->d_name) && nmatch(cmp->txt, reg->txt))
// 		{
// 			dstr_cutins_str(&cmp, "\0", cmp->strlen - 1);
// 			if (sh_isdir(cmp, 0))
// 				dstr_insert_str(&cmp, "/", cmp->strlen);
// 			dstr_insert_dstr(&cmp, next, cmp->strlen);
// 			rez.strings[++i] = cmp;
// 		}
// 	}
// 	return (rez);
// }

// char			cmp_dirreg(DSTRING *n_dir, DSTRING *cmp, DSTRING *reg, char fl)
// {
// 	DSTRING		*tmp;

// 	tmp = dstr_new(n_dir->txt);
// 	if (fl)
// 	{
// 		if (tmp->txt[tmp->strlen - 1] != '/')
// 			dstr_insert_str(tmp, "/", n_dir->strlen);
// 		dstr_insert_dstr(tmp, cmp, tmp->strlen);
// 		if (nmatch(tmp->txt, reg->txt))
// 		{
// 			dstr_del(&tmp);
// 			return (1);
// 		}
// 		dstr_del(&tmp);
// 	}
// 	else
// 		if (nmatch(cmp->txt, reg->txt))
// 			return (1);
// 	return (0);
// }

// t_darr			*cmp_dir_reg(DSTRING *n_dir, DSTRING *reg, char fl)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	DSTRING			*cmp;
// 	t_darr			*rez;
// 	int				i;

// 	dir = opendir(n_dir->txt);
// 	if (!dir)
// 		ft_putstr("/nNOT DIR\n");
// 	i = -1;
// 	ft_bzero(rez, sizeof(t_darr));
// 	while ((entry = readdir(dir)))
// 	{
// 		cmp = add_slash(entry->d_name, reg);
// 		if (is_strdot(entry->d_name) && cmp_dirreg(n_dir, cmp, reg, fl))
// 		{
// 			rez->strings[++i] = join_reg(n_dir, cmp, fl);
// 			rez->count++;
// 		}
// 		else
// 			dstr_del(&cmp);
// 	}
// 	return (rez);
// }

t_darr			get_regular_paths(DSTRING *buf)
{
	int			ij[2];
	int			slicefl[2];
	t_darr		rez;

	ij[0] = -1;
	ij[1] = -1;
	slicefl[0] = 0;
	slicefl[1] = 0;
	ft_bzero(&rez, sizeof(t_darr));
	while (++ij[0] < buf->strlen)
	{
		if (buf->txt[ij[0]] == '*')
			slicefl[0] = 1;
		if (buf->txt[ij[0]] == '/' && slicefl[0])
		{
			rez.strings[++ij[1]] = dstr_slice(buf, slicefl[1], ij[0] + 1);
			rez.count++;
			slicefl[1] = ij[0] + 1;
			slicefl[0] = 0;
		}
		if (ij[0] + 1 == buf->strlen)
			rez.strings[ij[1] + 1] = dstr_slice(buf, slicefl[1], ij[0] + 1);
	}
	return (rez);
}


typedef struct	s_regpath
{
	DSTRING		*path;
	char		fl;
}				t_regpath;

void			next(t_darr *tmp, t_darr reg_paths, int i)
{
	int			j;
	int			y;

	j = -1;
	while (++j < (*tmp).count)
	{
		while (i <= reg_paths.count)
		{
			dstr_insert_dstr((*tmp).strings[j], reg_paths.strings[i++], (*tmp).strings[j]->strlen);
		}
	}
}

char			*sh_readline(t_envp *env)
{
	DSTRING	*buf;
	t_indch	indch;
	char	*move;
	char	fl;
	char	asterics;

	buf = dstr_new(0);
	indch.ind = 0;
	fl = 0;
	asterics = 0;
	while (1)
	{
		if (fl == 0)
			indch.ch = ft_getch();
		// printf("0x%hhX == [%c]\n", indch.ch, (indch.ch < ' ' || indch.ch > '~') ? '$' : indch.ch);
		if (indch.ch == (char)0x04)
			break ; // вернуть строку "exit()"
		if (indch.ch == '\n')
			break ;
		else if ((indch.ch == BAKSP || indch.ch == DEL) && (fl = 0) == 0)
			indch.ind = sh_del_char(&buf, indch.ind, indch.ch);
		else if (ft_isprint(indch.ch) && (fl = 0) == 0)
			dstr_insert_ch(buf, indch.ch, indch.ind++);
		else if ((indch.ch == ESC) && (fl = 0) == 0)
			indch.ind = sh_esc(indch.ind, buf->strlen);
		if (indch.ch == TAB && asterics == 0 && fl++ == 0)
			indch = sh_tab(&buf, indch.ind, env);
		if (indch.ch == '*')
			asterics++;
		if (indch.ch == TAB && asterics)
		{
			indch.ind = astr(&buf);
			fl = 0;
			asterics = 0;
		}
		sh_rewrite(buf, indch.ind);
	}
	char *rez = ft_strdup(buf->txt);
	dstr_del(&buf);
	return (rez);
}
