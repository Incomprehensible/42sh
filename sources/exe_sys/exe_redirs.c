/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:02:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:08:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int				exe_redir_do(int fdl, int fdr, char *file, int flags)
{
	const mode_t	mode = 0644;

	if (file)
		fdr = open(file, flags, mode);
	dup2(fdr, fdl);
	close(fdr);
	return (1);
}

void			exe_redir_ex(REDIRECT *rdr)
{
	int			fd;
	int			file_flag;
	mode_t		mode;

	if ((fd = rdr->fdr) >= 0)
	{
		mode = 0644;
		file_flag = rdr->type != r_rdr ? O_CREAT : 0;
		rdr->type == r_rdr ? file_flag |= O_RDONLY | O_TRUNC : 0;
		rdr->type == w_rdr ? file_flag |= O_WRONLY | O_TRUNC : 0;
		rdr->type == rw_rdr ? file_flag |= O_RDWR : 0;
		rdr->type == a_rdr ? file_flag |= O_APPEND | O_WRONLY : 0;
		if (rdr->fdl == -21)
		{
			if (rdr->file)
				fd = open(rdr->file, file_flag, mode);
			dup2(fd, STDOUT_FILENO);
			// close(fd);									??
			// if (rdr->file)								??
			// fd = open(rdr->file, file_flag, mode);		??
			dup2(fd, STDERR_FILENO);
			close(fd);
			return ;
		}
		exe_redir_do(rdr->fdl, rdr->fdr, rdr->file, file_flag);
	}
	else if (fd == -42)
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
