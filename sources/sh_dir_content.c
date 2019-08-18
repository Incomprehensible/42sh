/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_dir_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 14:40:25 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/18 13:14:41 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"
#include <dirent.h>

size_t			sh_count_file(const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			rez;

	dir = opendir(path);
	while ((entry = readdir(dir)) != NULL)
		++rez;
	closedir(dir);
	return (rez);
}

void			free_darr_n(DSTRING **darr, const size_t size)
{
	size_t		i;

	i = -1;
	while (++i < size)
		dstr_del(&(darr[i]));
}

void			free_darr(DSTRING **darr)
{
	int		size;

	size = -1;
	while (darr[size])
	{
		printf("%d", size);
		dstr_del(&(darr[size]));
	}
}

char		sh_check_dot(const DSTRING *path)
{
	if (path->strlen == 1 && path->txt[0] == '.')
		return (0);
	if (path->strlen == 2 && path->txt[1] == '.')
		return (0);
	return (1);
}

static	char		sh_insert_slash(const char *path, const DSTRING *name)
{
	DSTRING			*tmp;

	if (!sh_check_dot(name))
		return (0);
	tmp = dstr_new(name->txt);
	dstr_insert_str(tmp, "/", 0);
	dstr_insert_str(tmp, (char *)path, 0);
	if (sh_isdir(tmp, 0))
	{
		dstr_del(&tmp);
		return (1);
	}
	dstr_del(&tmp);
	return (0);
}

t_darr				sh_dir_content(char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	t_darr			rez;
	size_t			i;

	dir = opendir(path);
	if ((rez.count = 0) == 0 && !dir)
	{
		ft_putendl("\n NO DIR\n");
		return (rez);
	}
	i = 0;
    rez.maxlen = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		rez.strings[i] = dstr_new(entry->d_name);
		if (sh_insert_slash(path, rez.strings[i]))
			dstr_insert_str(rez.strings[i], "/", rez.strings[i]->strlen);
		if (rez.strings[i]->strlen > rez.maxlen)
			rez.maxlen = rez.strings[i]->strlen;
		rez.allsize += rez.strings[i++]->strlen;
		rez.count++;
	}
	closedir(dir);
	return (rez);
}
