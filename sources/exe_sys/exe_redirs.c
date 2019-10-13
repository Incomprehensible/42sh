/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:02:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/13 04:39:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"

int				exe_redir_do(int fdl, int fdr, char *file, int flags)
{
	const mode_t	mode = 0644;

	if (file)
		fdr = open(file, flags, mode);
	dup2(fdr, fdl);
	close(fdr);
	return (1);
}

void			exe_redir_type(int *file_flag, t_rdtype tp)
{
	*file_flag = tp != r_rdr ? O_CREAT : 0;
	tp == r_rdr ? *file_flag |= O_RDONLY | O_NOCTTY: 0;
	tp == w_rdr ? *file_flag |= O_WRONLY | O_TRUNC | O_NOCTTY: 0;
	tp == rw_rdr ? *file_flag |= O_RDWR : 0;
	tp == a_rdr ? *file_flag |= O_APPEND | O_WRONLY : 0;
}

void			exe_rdr_heredoc_pipes(DSTRING *buff, int *fd)
{
	pipe(fd);
	dup2(fd[0], 0);
	write(fd[1], buff->txt, buff->strlen);
	close(fd[1]);
	close(fd[0]);
}

void			exe_rdr_heredocument(REDIRECT *rdr)
{
	char	*line;
	DSTRING	*buff;
	int		fd[2];

	buff = dstr_new("");
	while ((line = tmp_readline("heredoc> ")) || 1)
	{
		if (!line)
		{
			dstr_del(&buff);
			return ;
		}
		if (ft_strequ(line, rdr->file))
		{
			free(line);
			break;
		}
		dstr_insert_str(buff, line, buff->strlen);
		dstr_insert_ch(buff, '\n', buff->strlen);
		free(line);
	}
	exe_rdr_heredoc_pipes(buff, fd);
	dstr_del(&buff);
}

void			exe_redir_ex(REDIRECT *rdr)
{
	int				fd;
	int				file_flag;
	const mode_t	mode = 0644;

	if (rdr->type == herd)
		exe_rdr_heredocument(rdr);
	else if ((fd = rdr->fdr) >= 0)
	{
		exe_redir_type(&file_flag, rdr->type);
		if (rdr->fdl == -21)
		{
			if (rdr->file)
				fd = open(rdr->file, file_flag, mode);
			dup2(fd, STDOUT_FILENO);
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
