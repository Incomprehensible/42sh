/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_get_cmd_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:41:28 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/20 21:32:05 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"

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

static int		is_ex(char *path, char *name)
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
	while ((entry = readdir(dir)) != NULL)
	{
		ex = is_ex(path, entry->d_name);
		if (is_sysdir(entry->d_name, "") || !(!ex || ex == -2))
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
	t_dyn_string	*tmp;

	if ((sl = dstrrchr(bufffer->dir, '/')) == -1)
		return (0);
	tmp = dstr_nerr(bufffer->buf->txt);
	bufffer->dir = dstr_scerr(&tmp, bufffer->cut, sl + 1);
	bufffer->sub = dstr_scerr(&tmp, 0, end_cut(bufffer->dir->txt, 0, ' '));
	dstr_del(&tmp);
	return (1);
}
