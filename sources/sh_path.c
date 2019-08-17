/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:45:13 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 18:46:04 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

char			sh_isdir(DSTRING *buf, ssize_t start_dir)
{
	DIR			*dir;
	DSTRING		*tmp;
	
	tmp = dstr_slice(buf, start_dir, buf->strlen);
	if (tmp->txt[0] != '.' && tmp->txt[0] != '/')
	{
		if (ft_strncmp(tmp->txt, "./", 2))
			dstr_insert_str(tmp, "./", 0);
	}
	dir = opendir(tmp->txt);
	if (!dir)
	{
		dstr_del(&tmp);
		return (0);
	}
	dstr_del(&tmp);
	closedir(dir);
	return (1);
}

static char            sh_get_path_help(char *str)
{
   	DIR			*dir;

    dir = opendir(str);
    if (!dir)
        return (0);
    closedir(dir);
    return (1);
}

DSTRING			*sh_get_path(DSTRING *buf, size_t start_dir)
{
	DSTRING		*path1;
	DSTRING		*path2;
	DIR			*dir;
	
	path1 = dstr_slice(buf, start_dir, buf->strlen);
    if (sh_get_path_help(path1->txt))
        return (path1);	
	dstr_insert_str(path1, "./", 0);
	path2 = dstr_slice(path1, 0, dstrrchr(path1, '/'));
    if (sh_get_path_help(path1->txt))
    {
        dstr_del(&path2);
        return (path1);
    }
    if (sh_get_path_help(path2->txt))
    {
        dstr_del(&path1);
        return (path2);
    }
	return (dstr_new("."));
}
