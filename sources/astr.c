/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 06:51:22 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/25 09:47:16 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"


int		nmatch(char *s1, char *s2)
{
	if ((*s1 != '\0') && (*s2 == '*'))
		return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
	else if (*s1 == *s2 && *s2 != '\0')
		return (nmatch(s1 + 1, s2 + 1));
	else if (*s1 == '\0' && *s2 == '\0')
		return (1);
	else if (*s1 == '\0' && *s2 == '*')
		return (nmatch(s1, s2 + 1));
	return (0);
}

DSTRING			*cut_reg(DSTRING *buf)
{
	ssize_t		ind;

	ind = dstrrchr(buf, ' ');
	return(dstr_slice(buf, ind + 1, buf->strlen));
}

DSTRING		*cutreg(DSTRING *reg)
{
	int		i;
	int		fl;
	DSTRING	*rez;

	i = -1;
	fl = 0;
	while (reg->txt[++i])
	{
		if (reg->txt[i] == '*')
			fl = 1;
		if (reg->txt[i] == '/' && fl)
		{
			rez = dstr_slice(reg, 0, i + 1);
			break ;
		}
		if (reg->strlen == i + 1 && fl)
			rez = reg;
	}
	if (!rez)
		rez = dstr_new("");
	return (rez);
}

typedef struct	s_regpath
{
	DSTRING		*path;
	char		fl;
}				t_regpath;

t_regpath		get_regpath(DSTRING *reg)
{
	int			i;
	int			fl;
	DSTRING		*path;
	t_regpath	rez;

	i = -1;
	fl = 1;
	path = dstr_new("");
	while (++i < reg->strlen)
	{
		if (reg->txt[i] == '*')
			fl = 0;
		if (reg->txt[i] == '/' && fl)
		{
			if (path->strlen)
				dstr_del(&path);
			path = dstr_slice(reg, 0, i + 1);
		}
	}
	if (!path->strlen)
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

DSTRING			*add_slash(char *str, DSTRING *reg)
{
	DSTRING		*rez;

	rez = dstr_new(str);
	if (reg->txt[reg->strlen - 1] == '/')
		dstr_insert_str(rez, "/", rez->strlen);
	return (rez);
}

DSTRING			*join_reg(DSTRING *n_dir, DSTRING *cmp, char fl)
{
	DSTRING		*tmp;

	if (fl)
	{
		tmp = dstr_new(n_dir->txt);
		if (tmp->txt[tmp->strlen - 1] != '/')
			dstr_insert_str(tmp, "/", tmp->strlen);
		dstr_insert_dstr(tmp, cmp, n_dir->strlen);
		return (tmp);
	}
	return (cmp);
}

char			cmp_dirreg(DSTRING *n_dir, DSTRING *cmp, DSTRING *reg, char fl)
{
	DSTRING		*tmp;

	tmp = dstr_new(n_dir->txt);
	if (fl)
	{
		if (tmp->txt[tmp->strlen - 1] != '/')
			dstr_insert_str(tmp, "/", n_dir->strlen);
		dstr_insert_dstr(tmp, cmp, tmp->strlen);
		if (nmatch(tmp->txt, reg->txt))
		{
			dstr_del(&tmp);
			return (1);
		}
		dstr_del(&tmp);
	}
	else
		if (nmatch(cmp->txt, reg->txt))
			return (1);
	return (0);
}

char		is_strdot(const char *path)
{
	if (ft_strlen(path) == 1 && path[0] == '.')
		return (0);
	if (ft_strlen(path) == 2 && path[1] == '.')
		return (0);
	return (1);
}

int			cmp(t_darr *rez, int i, t_regpath path, DSTRING *reg)
{
	DIR				*dir;
	struct dirent	*entry;
	DSTRING			*cmp;
	char			fl;
	
	dir = opendir(path.path->txt);
	if (!dir)
		return (i);
	fl = 0;
	while ((entry = readdir(dir)))
	{
		cmp = add_slash(entry->d_name, reg);
		if (is_strdot(entry->d_name) && cmp_dirreg(path.path, cmp, reg, path.fl))
		{
			rez->strings[i++] = join_reg(path.path, cmp, path.fl);
			rez->count++;
			fl = 1;
		}
		else
			dstr_del(&cmp);
	}
	if (!fl)
		return i + 1;
	return (i);
}

void		addreg(t_darr *rez, DSTRING *r, DSTRING *reg)
{
	int		i;
	DSTRING	*slice;

	i = 0;
	slice = dstr_slice(reg, r->strlen, reg->strlen + 1);
	if (!slice->strlen)
		return ;
	while (i < rez->count)
	{
		dstr_cutins_dstr(&rez->strings[i], slice, rez->strings[i]->strlen);
		i++;
	}
	dstr_del(&slice);
}

char			chek_astr(DSTRING *reg)
{
	int			i;

	i = -1;
	while (reg->txt[++i])
		if (reg->txt[i] == '*')
			return (0);
	return (1);
}

void		loop(DSTRING *reg, int i, t_darr *rez, int iter)
{
	DSTRING		*r;
	t_regpath	path;

	if (reg && chek_astr(reg))
		return ;
	if (!reg)
		return ;
	r = cutreg(reg);
	path = get_regpath(r);
	i = cmp(rez, i, path, r);
	addreg(rez, r, reg);
	if (dstrrchr(rez->strings[iter], '*'))
		loop(rez->strings[iter], rez->count, &(*rez), iter + 1);
}

void		fill_buf(DSTRING **buf, t_darr rez)
{
	int		ind;
	int		j;

	ind = -1;
	while ((*buf)->txt[++ind])
		if ((*buf)->txt[ind] == ' ')
			break ;
	j = -1;
	while (++j < rez.count)
		if (dstrrchr(rez.strings[j], '*') == -1)
			break ;
	dstr_cutins_str(buf, "", ind); 
	while (j < rez.count)
	{
		dstr_insert_str((*buf), " ", (*buf)->strlen);
		dstr_insert_dstr((*buf), rez.strings[j++], (*buf)->strlen);
	}
}

int		astr(DSTRING **buf)
{
	DSTRING		*reg;
	t_darr		rez;
	t_indch		i;
	
	reg = cut_reg(*buf);
	ft_bzero(&rez, sizeof(t_darr));
	loop(reg, 0, &rez, 0);
	fill_buf(buf, rez);
	return ((*buf)->strlen);
}
