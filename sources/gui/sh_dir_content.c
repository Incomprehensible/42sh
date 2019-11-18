/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_dir_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 14:40:25 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/17 15:34:27 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"
#include <dirent.h>

char				sh_check_dot(const DSTRING *path)
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
	tmp = dstr_nerr(name->txt);
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

char				is_sysdir(char *name, char *sub)
{
	if (sub[0] == '.')
	{
		if (name[0] == '.' && name[1] == '\0')
			return (1);
		if (name[0] == '.' && name [1] == '.')
			return (1);
		return (0);
	}
	if (name[0] == '.')
		return (1);
	return (0);
}

t_darr				sh_dir_content(char *path, DSTRING *sub)
{
	struct dirent	*entry;
	DIR				*dir;
	t_darr			rez;
	size_t			i;

	i = 0;
	rez.count = 0;
	rez.maxlen = 0;
	dir = opendir(path);
	while ((entry = readdir(dir)) != NULL)
	{
		if (is_sysdir(entry->d_name, sub->txt))
			continue ;
		rez.strings[i] = dstr_nerr(entry->d_name);
		if (sh_insert_slash(path, rez.strings[i]))
			dstr_insert_str(rez.strings[i], "/", rez.strings[i]->strlen);
		if (rez.strings[i]->strlen > (ssize_t)rez.maxlen)
			rez.maxlen = rez.strings[i]->strlen;
		rez.allsize += rez.strings[i++]->strlen;
		rez.count++;
	}
	closedir(dir);
	return (rez);
}
