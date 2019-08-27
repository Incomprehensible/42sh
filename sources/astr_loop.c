/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astr_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 00:51:57 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/27 12:06:09 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

static t_regpath	help_get_regpath(const int fl, DSTRING *path)
{
	t_regpath	rez;

	if (fl != 2)
	{
		rez.fl = 0;
		rez.path = dstr_new(".");
	}
	else
	{
		rez.fl = 1;
		rez.path = path;
	}
	return (rez);
}

t_regpath		get_regpath(DSTRING *reg)
{
	int			i;
	int			fl;
	int			fl2;
	DSTRING		*path;

	i = -1;
	fl = 1;
	fl2 = 1;
	path = NULL;
	while (++i < reg->strlen)
	{
		if (reg->txt[i] == '*' || reg->txt[i] == '?' || reg->txt[i] == '[')
			fl = 0;
		if (reg->txt[i] == '/' && fl)
		{
			if (path)
				dstr_del(&path);
			path = dstr_slice(reg, 0, i + 1);
			fl2 = 2;
		}
	}
	return (help_get_regpath(fl2, path));
}

static int			cmp(t_astr *rez, int i, t_regpath pth, DSTRING *reg)
{
	DIR				*dir;
	struct dirent	*entry;
	DSTRING			*cmp;
	char			fl;

	dir = opendir(pth.path->txt);
	if (!dir)
		return (i);
	fl = 0;
	while ((entry = readdir(dir)))
	{
		cmp = add_slash(entry->d_name, reg);
		if (is_strdot(entry->d_name) && cmp_dirreg(pth.path, cmp, reg, pth.fl))
		{
			rez->strings[i++] = join_reg(pth.path, cmp, pth.fl);
			rez->count++;
			fl = 1;
		}
		dstr_del(&cmp);
	}
	closedir(dir);
	if (!fl)
		return (i + 1);
	return (i);
}

static void		addreg(t_astr *rez, DSTRING *r, DSTRING *reg, int j)
{
	int		i;
	DSTRING	*slice;

	i = j;
	slice = dstr_slice(reg, r->strlen, reg->strlen + 1);
	if (!slice->strlen)
	{
		dstr_del(&slice);
		return ;
	}
	while (i < rez->count)
	{
		dstr_insert_dstr(rez->strings[i], slice, rez->strings[i]->strlen);
		i++;
	}
	dstr_del(&slice);
}

void		loop(DSTRING *reg, int i, t_astr *rez, const int itr)
{
	DSTRING		*r;
	int			j;
	t_regpath	path;

	if ((reg && chek_astr(reg)) || !reg)
		return ;
	r = slice_reg(reg);
	path = get_regpath(r);
	j = i;
	i = cmp(rez, i, path, r);
	addreg(rez, r, reg, j);
	dstr_del(&path.path);
	dstr_del(&r);
	if (dstrrchr(rez->strings[itr], '*') != -1)
		loop(rez->strings[itr], rez->count, &(*rez), itr + 1);
}
