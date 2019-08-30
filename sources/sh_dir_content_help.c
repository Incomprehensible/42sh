/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_dir_content_help.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 08:07:20 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:08:10 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"
#include <dirent.h>

size_t				sh_count_file(const char *path)
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

void				free_darr_n(DSTRING **darr, const size_t size)
{
	size_t		i;

	i = -1;
	while (++i < size)
		dstr_del(&(darr[i]));
}

void				free_darr(DSTRING **darr)
{
	int		size;

	size = -1;
	while (darr[size])
	{
		printf("%d", size);
		dstr_del(&(darr[size]));
	}
}
