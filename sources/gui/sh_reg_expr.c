/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_reg_expr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 00:51:57 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/12/17 17:44:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

t_regpath		help_get_regpath(const int fl, DSTRING *path)
{
	t_regpath	rez;

	if (fl != 2)
	{
		rez.fl = 0;
		rez.path = dstr_nerr(".");
	}
	else
	{
		rez.fl = 1;
		rez.path = path;
	}
	return (rez);
}

static int		cmp(t_astr *rez, int i, t_regpath pth, DSTRING *reg)
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
	size_t		i;
	DSTRING		*slice;

	i = j;
	slice = dstr_serr(reg, r->strlen, reg->strlen + 1);
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

void			loop(DSTRING *reg, int i, t_astr *rez, const int itr)
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

int				reg_expr(DSTRING **buf, t_indch *indch, ENV *env)
{
	t_astr		rez;
	t_buf		buffer;

	if (is_reg(*buf) == -1)
		return (0);
	buffer = slicer_reg(buf);
	ft_bzero(&rez, sizeof(t_astr));
	loop(buffer.sub, 0, &rez, 0);
	if (rez.count > 0)
		buffer.val = get_dstr_rez(rez);
	unite_buf(buf, &buffer);
	dstr_del(&buffer.val);
	indch ? indch->fl = 0 : 0;
	indch ? indch->ind = (*buf)->strlen : 0;
	if (!rez.count)
	{
		free_t_astr(&rez);
		return (0);
	}
	free_t_astr(&rez);
	if (env)
		return (1);
	return (1);
}
