/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_history_rl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 23:39:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 23:49:14 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "fcntl.h"

void			add_buf_history(DSTRING *buf)
{
	int				ind;
	char *const		trimmed = ft_strtrim(buf->txt);
	size_t			len;

	ind = S_DARR_STRINGS - (g_histr.count + 1);
	if (!trimmed)
		return ;
	if (ind > 0)
	{
		len = ft_strlen(trimmed);
		if (len && !(ft_strequ(trimmed, "exit")))
		{
			g_histr.strings[ind] = dstr_nerr(trimmed);
			g_histr.count++;
			g_histr.allsize += len;
		}
	}
	free(trimmed);
}

void			save_histr(ENV *envr)
{
	int		fd;
	int		ind;
	size_t	count;

	if (!g_histr.count || ((fd = get_history_fd(O_WRONLY \
	| O_TRUNC, "REWRITE_HISTORY: File error", envr)) < 0))
		return ;
	if (g_histr.count > 1000)
		ind = S_DARR_STRINGS - (g_histr.count - 1000);
	else
		ind = S_DARR_STRINGS - 1;
	count = 0;
	while (count <= 1001 && g_histr.strings[ind])
	{
		write(fd, g_histr.strings[ind]->txt, g_histr.strings[ind]->strlen);
		write(fd, "\n", 1);
		ind--;
		count++;
	}
	close(fd);
}

void			del_history_buf(t_darr *histr)
{
	int		i;
	size_t	count;

	count = 0;
	i = S_DARR_STRINGS - histr->count - 1;
	while (count < histr->count)
	{
		++count;
		dstr_del(&(histr->strings[i--]));
	}
}
