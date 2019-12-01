/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_dir_content_help.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 08:07:20 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/23 14:07:04 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"
#include <dirent.h>

void				free_t_astr(t_astr *darr)
{
	size_t	i;

	i = -1;
	if (darr->count == (size_t)-1)
		return ;
	while (++i < darr->count)
		dstr_del(&darr->strings[i]);
	darr->count = 0;
}

void				free_t_darr(t_darr *darr)
{
	size_t	i;

	i = -1;
	if (darr->count == (size_t)-1)
		return ;
	while (++i < darr->count)
		dstr_del(&darr->strings[i]);
	darr->count = 0;
	darr->allsize = 0;
	darr->maxlen = 0;
}

void				free_darr_re(DSTRING **darr, const int size)
{
	int		i;
	int		c;

	i = S_DARR_STRINGS - 1;
	c = -1;
	while (++c < size)
		dstr_del(&(darr[i--]));
}
