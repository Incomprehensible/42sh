/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_get_file_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:34:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 16:04:09 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"

static char		get_path_help(char *str)
{
	DIR			*dir;

	dir = opendir(str);
	if (!dir)
		return (0);
	closedir(dir);
	return (1);
}

void			dell_slash(DSTRING **sub)
{
	int			i;

	i = -1;
	while ((*sub)->txt[++i])
		if ((*sub)->txt[i] == '\\')
			dstr_del_char(sub, i);
}

DSTRING			*get_path(DSTRING *file, int fl)
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
