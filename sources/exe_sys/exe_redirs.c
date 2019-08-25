/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:02:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/25 03:07:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

void			exe_redir_ex(REDIRECT *rdr)
{
	int			fd;
	int			file_flag;
	mode_t		mode;

	if ((fd = rdr->fdr) >= 0)
	{
		mode = 0644;
		file_flag = O_CREAT;
		rdr->type == r_rdr ? file_flag |= O_RDONLY | O_TRUNC : 0;
		rdr->type == w_rdr ? file_flag |= O_WRONLY | O_TRUNC : 0;
		rdr->type == rw_rdr ? file_flag |= O_RDWR : 0;
		rdr->type == a_rdr ? file_flag |= O_APPEND | O_WRONLY : 0;
		if (rdr->fdl == -2)
		{
			if (rdr->file)
				fd = open(rdr->file, file_flag, mode);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (rdr->file)
				fd = open(rdr->file, file_flag, mode);
			dup2(fd, STDERR_FILENO);
			close(fd);
			return ;
		}
		if (rdr->file)
			fd = open(rdr->file, file_flag, mode);
		dup2(fd, rdr->fdl);
		close(fd);
	}
	else if (fd == -2)
		close(rdr->fdl);
}

void			exe_redir_save420(t_dlist *redrs)
{
	REDIRECT	*rdr;

	while (redrs)
	{
		rdr = (REDIRECT *)redrs->content;
		dup2(rdr->fdl, rdr->fdl + 420);
		redrs = redrs->next;
	}
}

void			exe_redir_load420(t_dlist *redrs)
{
	REDIRECT	*rdr;

	while (redrs)
	{
		rdr = (REDIRECT *)redrs->content;
		dup2(rdr->fdl + 420, rdr->fdl);
		close(rdr->fdl + 420);
		redrs = redrs->next;
	}
}
