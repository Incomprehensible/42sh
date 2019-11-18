/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:51:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 02:25:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"

int				is_all_space(DSTRING *buf, t_indch *indch)
{
	int		i;

	i = -1;
	while (buf->txt[++i])
		if (buf->txt[i] != ' ')
			return (0);
	indch->ind++;
	dstr_insert_ch(buf, ' ', buf->strlen);
	return (1);
}

int				end_cut(char *str, int start, char ch)
{
	while (str[start] && str[start] != ch)
	{
		if (str[start] == '\\')
			++start;
		++start;
	}
	return (start);
}

int				is_ex(char *path, char *name)
{
	char		*tmp;
	int			rez;

	tmp = ft_strjoin(path, name);
	rez = sys_is_ex_bin(tmp);
	ft_strdel(&tmp);
	return (rez);
}

t_darr			get_executable_files(char *path)
{
	struct dirent	*entry;
	DIR				*dir;
	t_darr			rez;
	int				i;
	int				ex;

	i = -1;
	dir = opendir(path);
	ft_bzero(&rez, sizeof(t_darr));
	while((entry = readdir(dir)) != NULL)
	{
		
		ex = is_ex(path, entry->d_name);
		if (is_sysdir(entry->d_name, "") && (ex != -3 || ex != -2))
			continue ;
		rez.strings[++i] = dstr_nerr(entry->d_name);
		if (ex == -2)
			dstr_insert_ch(rez.strings[i], '/', rez.strings[i]->strlen);
		if ((ssize_t)rez.maxlen < rez.strings[i]->strlen)
			rez.maxlen = rez.strings[i]->strlen;
		rez.allsize += rez.strings[i]->strlen;
		rez.count++;
	}
	closedir(dir);
	return (rez);
}

int				is_executable(t_buf *bufffer)
{
	int				sl;
	DSTRING			*tmp;
	if ((sl = dstrrchr(bufffer->sub, '/')) == -1)
		return (0);
	tmp = dstr_nerr(bufffer->buf->txt);
	bufffer->dir = dstr_scerr(&tmp, bufffer->cut, sl + 1);
	bufffer->sub = dstr_scerr(&tmp, 0, end_cut(bufffer->dir->txt, 0, ' '));
	dstr_del(&tmp);
	return (1);
}

t_darr			sh_get_cmd(t_buf *buffer, ENV *env)
{
	t_darr			allcmd;
	t_darr			overlap;

	if (is_executable(buffer))
		allcmd = get_executable_files(buffer->dir->txt);
	else
		allcmd = env_get_bins(env);
	overlap = sh_cmp_darr(allcmd, buffer->sub);
	free_t_darr(&allcmd);
	return (overlap);
}

static char		get_path_help(char *str)
{
	DIR			*dir;

	dir = opendir(str);
	if (!dir)
		return (0);
	closedir(dir);
	return (1);
}

static DSTRING			*get_path(DSTRING *file, int fl)
{
	DSTRING		*tmp;
	
	if (fl && get_path_help(file->txt))
		return (dstr_nerr(file->txt));
	if (fl && !get_path_help(file->txt))
		return (dstr_nerr("error"));
	if (get_path_help(file->txt))
		return (dstr_nerr(file->txt));
	dstr_insert_str(file, "./", 0);
	if (get_path_help(file->txt))
		return (dstr_nerr(file->txt));
	tmp = dstr_serr(file, 0, dstrrchr(file, '/'));
	if (get_path_help(tmp->txt))
		return (tmp);
	dstr_del(&tmp);
	return (dstr_nerr("."));
}

void			dell_slash(DSTRING **sub)
{
	int			i;

	i = -1;
	while ((*sub)->txt[++i])
		if ((*sub)->txt[i] == '\\')
			dstr_del_char(sub, i);
}

t_darr			sh_get_file(DSTRING *dir, DSTRING **sub, int ind_inp, ENV *env)
{
	t_darr		content;
	t_darr		overlap;
	DSTRING		*path;

	ind_inp = 0;
	env = 0;
	if (!dir)
		path = get_path(*sub, 0);
	else
		path = get_path(dir, 1);
	if (!ft_strequ(path->txt, "error"))
		content = sh_dir_content(path->txt, (*sub));
	else
	{
		overlap.count = -1;
		return (overlap);
	}
	dell_slash(sub);
	overlap = sh_cmp_darr(content, *sub ? *sub : dir);
	free_t_darr(&content);
	dstr_del(&path);
	return (overlap);
}

int			is_var(t_buf *buffer, t_darr *allvar, ENV *env)
{
	DSTRING		*value;
	DSTRING		*tmp;
	int			sl;

	if ((sl = dstrrchr(buffer->sub, '/')) == -1)
		return (0);
	tmp = dstr_scerr(&(buffer->sub), 0, sl);
	dstr_insert_dstr(buffer->end, buffer->sub, 0);
	value = env_get_variable(tmp->txt, env);
	if (!value->strlen)
		return (0);
	allvar->strings[0] = value;
	allvar->count = 1;
	dstr_del_char(&(buffer->begin), buffer->begin->strlen - 1);
	dstr_del(&tmp);
	return (1);
}

t_darr			sh_get_var(t_buf *buffer, ENV *env)
{
	t_darr			allvar;
	t_darr			overlap;

	if (is_var(buffer, &allvar, env))
		return (allvar);
	if (buffer->sub->txt[0] == '{')
		allvar = env_get_keys(env, "{");
	else
		allvar = env_get_keys(env, "");
	overlap = sh_cmp_darr(allvar, buffer->sub);
	free_t_darr(&allvar);
	return (overlap);
}

t_darr			get_overlap(t_buf *buffer, t_indch *indch, ENV *env)
{
	t_darr		overlap;

	overlap.count = 0;
	if (is_all_space(buffer->buf, indch) && (overlap.count = -1))
		return (overlap);
	else if (indch->type_inp == 0)
		return (sh_get_cmd(buffer, env));
	else if (indch->type_inp == 1)
		return (sh_get_file(buffer->dir, &(buffer->sub), indch->ind_inp, env));
	else if (indch->type_inp == 2)
		return (sh_get_var(buffer, env));
	return (overlap);
}

int				insert_space(DSTRING **buf, t_indch indch)
{
	if (!sh_isdir(*buf, indch.ind_inp))
		dstr_insert_ch(*buf, ' ', (*buf)->strlen);
	return (1);
}

void			check_spic_sumbols(DSTRING **str)
{
	int			i;

	i = -1;
	while (++i < (*str)->strlen)
		if (ft_strchr(SPEC_SYMBOLS, (*str)->txt[i]))
			dstr_insert_ch((*str), '\\', i++);
}

void			subst_val(t_buf *buffer, t_darr overlap, t_name_ind n_ind)
{
	DSTRING		*new;

	new = dstr_nerr("");
	buffer->val = overlap.strings[n_ind.ind];
	dstr_insert_dstr(new, buffer->begin, 0);
	if (buffer->dir)
		dstr_insert_dstr(new, buffer->dir, new->strlen);
	if (!n_ind.road && n_ind.indch.type_inp != 2)
		check_spic_sumbols(&overlap.strings[n_ind.ind]);
	dstr_insert_dstr(new, overlap.strings[n_ind.ind], new->strlen);
	dstr_insert_dstr(new, buffer->end, new->strlen);
	sh_rewrite(n_ind.indch.prompt, new, buffer->cut < buffer->slash ? \
	buffer->slash + buffer->val->strlen : buffer->cut + buffer->val->strlen);
	dstr_del(&new);
}

t_name_ind	tab_loop(t_darr overlap, t_buf *buffer, int fl, t_name_ind n_ind)
{
	if (!fl && overlap.count > 1)
		put_col(overlap, buffer->buf, n_ind.indch);
	else if (overlap.count > 1)
	{
		subst_val(buffer, overlap, n_ind);
		n_ind.ind++;
	}
	else if (overlap.count == 1)
		subst_val(buffer, overlap, n_ind);
	if ((size_t)n_ind.ind == overlap.count)
	{
		n_ind.ind = 0;
		n_ind.road = 1;
	}
	return (n_ind);
}

t_buf			slicer(DSTRING **buf, int cut, int slash)
{
	t_buf		new;

	new.buf = dstr_nerr((*buf)->txt);
	new.sub = NULL;
	new.dir = NULL;
	if (!slash)
		new.sub = dstr_scerr(buf, cut, end_cut((*buf)->txt, cut, ' '));
	else
	{
		new.sub = dstr_scerr(buf, slash, end_cut((*buf)->txt, cut, ' '));
		new.dir = dstr_scerr(buf, cut, slash);
	}
	new.begin = dstr_serr((*buf), 0, cut);
	new.end = dstr_serr((*buf), cut, (*buf)->strlen);
	new.val = NULL;
	new.cut = cut;
	new.slash = slash;
	return (new);	
}

void			write_in_buf(DSTRING **buf, t_buf *buffer)
{
	dstr_del(buf);
	if (buffer->val)
	{
		(*buf) = dstr_nerr("");
		dstr_insert_dstr((*buf), buffer->begin, 0);
		if (buffer->dir)
			dstr_insert_dstr((*buf), buffer->dir, (*buf)->strlen);
		dstr_insert_dstr((*buf), buffer->val, (*buf)->strlen);
		dstr_insert_dstr((*buf), buffer->end, (*buf)->strlen);
	}
	else
		(*buf) = dstr_nerr(buffer->buf->txt);
	dstr_del(&(buffer->buf));
	dstr_del(&(buffer->begin));
	dstr_del(&(buffer->end));
	dstr_del(&(buffer->sub));
	if (buffer->dir)
		dstr_del(&(buffer->dir));
}

int				move_carret(int ind_inp, int ind, t_buf buffer)
{
	if (buffer.dir && buffer.val)
		return (ind_inp + buffer.dir->strlen + buffer.val->strlen);
	else if (buffer.val)
		return (ind_inp + buffer.val->strlen);
	return (ind);
}

void			sh_tab(DSTRING **buf, t_indch *indch, ENV *env)
{
	t_name_ind		n_ind;
	t_darr			overlap;
	t_buf			buffer;

	ft_bzero(&n_ind, sizeof(t_name_ind));
	n_ind.indch = (*indch);
	n_ind.ind_name = indch->ind_inp;
	buffer = slicer(buf, indch->ind_inp, indch->ind_slash);
	overlap = get_overlap(&buffer, indch, env);
	if (overlap.count == (size_t)-1 && overlap.count)
	{
		write_in_buf(buf, &buffer);
		return ;
	}
	sort_darr(&overlap);
	while (1)
	{
		n_ind = tab_loop(overlap, &buffer, n_ind.fl++, n_ind);
		if (overlap.count <= 1 && insert_space(buf, (*indch)))
			break ;
		indch->ch = ft_getch();
		if (indch->ch != TAB && (indch->fl = 1))
			break ;
	}
	indch->ind = move_carret(indch->ind_inp, indch->ind, buffer);
	write_in_buf(buf, &buffer);
	free_t_darr(&overlap);
}
