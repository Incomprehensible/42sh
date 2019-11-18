/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:02:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/18 19:57:22 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

int				exe_redir_do(int fdl, int fdr, char *file, int flags)
{
	const mode_t	mode = 0644;

	if (file && (fdr = sys_file_op(file, 0, flags, (char *)1)) < 0)
		return (fdr);
	if (fdr != fdl)
	{
		if (dup2(fdr, fdl) < 0)
			return (-E_BADFD);
		close(fdr);
	}
	return (0);
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

int			exe_rdr_heredocument(REDIRECT *rdr, ENV *env)
{
	DSTRING			*line;
	DSTRING			*buff;
	DSTRING	const	*prompt = dstr_new("heredoc> ");
	int				fd[2];

	buff = dstr_new("");
	while ((line = sh_readline(prompt, env)) || 1)
	{
		if (!line)
		{
			dstr_del(&buff);
			dstr_del((DSTRING **)&prompt);
			return (-1);
		}
		if (ft_strequ(line->txt, rdr->file))
		{
			dstr_del(&line);
			break ;
		}
		dstr_insert_dstr(buff, line, buff->strlen);
		dstr_insert_ch(buff, '\n', buff->strlen);
		dstr_del(&line);
	}
	exe_rdr_heredoc_pipes(buff, fd);
	dstr_del(&buff);
	dstr_del((DSTRING **)&prompt);
	return (0);
}

int				exe_redir_ex(REDIRECT *rdr, ENV *envr)
{
	int				fd;
	int				file_flag;
	const mode_t	mode = 0644;

	if (rdr->type == herd)
	{
		if (exe_rdr_heredocument(rdr, envr) < 0)
			return (sys_perror("Heredocument io failed!", -1, 0));
	}
	else if ((fd = rdr->fdr) >= 0)
	{
		exe_redir_type(&file_flag, rdr->type);
		if (rdr->fdl == -21)
		{
			if (rdr->file && \
			(fd = sys_file_op(rdr->file, envr, file_flag, (char *)1)) < 0)
				return (fd);
			if (dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0)
				return (-E_BADFD);
			STDOUT_FILENO != fd && fd != STDERR_FILENO ? close(fd) : 0;
			return (0);
		}
		return (exe_redir_do(rdr->fdl, rdr->fdr, rdr->file, file_flag));
	}
	else if (fd == -42)
		close(rdr->fdl);
	return (0);
}

void			exe_redir_save420(t_dlist *redrs)
{
	REDIRECT	*rdr;

	while (redrs)
	{
		rdr = (REDIRECT *)redrs->content;
		dup2(rdr->fdl, rdr->fdl + SYS_REDR_OFFST);
		redrs = redrs->next;
	}
}

void			exe_redir_load420(t_dlist *redrs)
{
	REDIRECT	*rdr;

	while (redrs)
	{
		rdr = (REDIRECT *)redrs->content;
		dup2(rdr->fdl + SYS_REDR_OFFST, rdr->fdl);
		close(rdr->fdl + SYS_REDR_OFFST);
		redrs = redrs->next;
	}
}
