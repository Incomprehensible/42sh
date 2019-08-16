/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_dir_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 14:40:25 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/16 18:36:07 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"
#include <dirent.h>

size_t		sh_count_file(const char *path)
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

void		free_darr_n(DSTRING **darr, const size_t size)
{
	size_t		i;

	i = -1;
	while (i < size)
		dstr_del(&darr[size]);
}

void		free_darr(DSTRING **darr)
{
	size_t		size;

	size = -1;
	while (darr[++size])
		dstr_del(&darr[size]);
}

t_darr		sh_dir_content(const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	t_darr			rez;
	size_t			i;

	dir = opendir(path);
	if (!dir)
	{
		ft_putendl("\n NO DIR\n");
		return (rez);
	}
	rez.count = sh_count_file(path);
	i = 0;
    rez.maxlen = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		rez.strings[i] = dstr_new(entry->d_name);
		if (rez.strings[i]->strlen > rez.maxlen)
			rez.maxlen = rez.strings[i]->strlen;
		rez.allsize += rez.strings[i++]->strlen;
	}
	closedir(dir);
	return (rez);
}
