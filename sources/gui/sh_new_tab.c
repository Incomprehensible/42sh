/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_new_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:51:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/14 20:48:12 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"


int					is_all_space(DSTRING *buf, t_indch *indch)
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

int					end_cut(char *str, int start, char ch)
{
	while (str[start] && str[start] != ch)
	{
		if (str[start] == '\\')
			++start;
		++start;
	}
	return (start);
}

typedef struct	s_buf
{
	DSTRING		*begin;
	DSTRING		*end;
	DSTRING		*buf;
	DSTRING		*val;
	DSTRING		*sub;
	DSTRING		*dir;
	int			cut;
	int			slash;
}				t_buf;

t_darr				sh_get_cmd(DSTRING *cmd, int ind_inp, ENV *env)
{
	t_darr			allcmd;
	t_darr			overlap;

	allcmd = env_get_bins(env);
	overlap = sh_cmp_darr(allcmd, cmd);
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

static DSTRING			*get_path(DSTRING *file)
{
	DSTRING		*tmp;
	
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

t_darr			sh_get_file(DSTRING *file, int ind_inp, ENV *env)
{
	t_darr		content;
	t_darr		overlap;
	DSTRING		*path;

	path = get_path(file);
	content = sh_dir_content(path->txt);
	overlap = sh_cmp_darr(content, file);
	return (overlap);
}

t_darr			get_overlap(t_buf buffer, t_indch *indch, ENV *env)
{
	t_darr		overlap;

	overlap.count = 0;
	if (is_all_space(buffer.buf, indch) && (overlap.count = -1))
		return (overlap);
	else if (indch->type_inp == 0)
		return (sh_get_cmd(buffer.sub, indch->ind_inp, env));
	else if (indch->type_inp == 1)
		return (sh_get_file(buffer.sub, indch->ind_inp, env));
	return (overlap);
}

int				insert_space(DSTRING **buf, t_indch indch)
{
	if (!sh_isdir(*buf, indch.ind_inp))
		dstr_insert_ch(*buf, ' ', (*buf)->strlen);
	return (1);
}



void			subst_val(t_buf *buffer, t_darr overlap, int ind, DSTRING *prompt)
{
	DSTRING		*new;

	new = dstr_new("");
	buffer->val = overlap.strings[ind];
	dstr_insert_dstr(new, buffer->begin, 0);
	dstr_insert_dstr(new, buffer->dir, new->strlen);
	dstr_insert_dstr(new, overlap.strings[ind], buffer->begin->strlen);
	dstr_insert_dstr(new, buffer->end, new->strlen);
	sh_new_rewrite(prompt, new, buffer->cut + buffer->val->strlen);
	dstr_del(&new);
}

int			tab_loop(t_darr overlap, t_buf *buffer, int fl, t_name_ind n_ind)
{
	if (!fl && overlap.count > 1)
		put_col(overlap, buffer->buf, n_ind.indch);
	else if (overlap.count > 1)
		subst_val(buffer, overlap, n_ind.ind++, n_ind.indch.prompt);
	else if (overlap.count == 1)
		subst_val(buffer, overlap, 0, n_ind.indch.prompt);
	if ((size_t)n_ind.ind == overlap.count)
		n_ind.ind = 0;
	return (n_ind.ind);
}

t_buf			slicer(DSTRING **buf, int cut, int slash)
{
	t_buf		new;

	new.buf = dstr_nerr((*buf)->txt);
	new.sub = NULL;
	if (!slash)
		new.sub = dstr_scerr(buf, cut, end_cut((*buf)->txt, cut, ' '));
	else
	{
		new.dir = dstr_serr((*buf), cut, slash);
		dstr_scerr(buf, cut, end_cut((*buf)->txt, cut, ' '));
	}
	new.begin = dstr_serr((*buf), 0, cut);
	new.end = dstr_serr((*buf), cut, (*buf)->strlen);
	new.val = NULL;
	new.cut = cut;
	new.slash = slash;
	return (new);	
}

void			write_in_buf(DSTRING **buf, t_buf *buffer, int ind_inp)
{
	dstr_del(buf);
	(*buf) = dstr_nerr("");
	if (buffer->val)
	{
		dstr_insert_dstr((*buf), buffer->begin, 0);
		dstr_insert_dstr((*buf), buffer->val, (*buf)->strlen);
		dstr_insert_dstr((*buf), buffer->end, (*buf)->strlen);
	}
	else
	(*buf) = dstr_nerr(buffer->buf->txt);
	dstr_del(&(buffer->buf));
	dstr_del(&(buffer->begin));
	dstr_del(&(buffer->end));
	dstr_del(&(buffer->sub));
}

void			sh_new_tab(DSTRING **buf, t_indch *indch, ENV *env)
{
	int				fl;
	t_name_ind		n_ind;
	t_darr			overlap;
	t_buf			buffer;

	fl = 0;
	n_ind.ind = 0;
	n_ind.indch = (*indch);
	buffer = slicer(buf, indch->ind_inp, indch->ind_slash);
	overlap = get_overlap(buffer, indch, env);
	if (overlap.count == (size_t)-1)
		return ;
	n_ind.ind_name = indch->ind_inp;
	sort_darr(&overlap);
	while (1)
	{
		n_ind.ind = tab_loop(overlap, &buffer, fl++, n_ind);
		if (overlap.count <= 1 && insert_space(buf, (*indch)))
			break ;
		indch->ch = ft_getch();
		if (indch->ch != TAB && (indch->fl = 1))
			break ;
	}
	indch->ind = buffer.val ? indch->ind_inp + buffer.val->strlen : indch->ind;
	write_in_buf(buf, &buffer, indch->ind_inp);
	free_t_darr(&overlap);
}
