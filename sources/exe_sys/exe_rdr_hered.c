/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_rdr_hered.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:17:16 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/01 19:06:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

void		exe_rdr_heredoc_pipes(DSTRING *buff)
{
	int		fd[2];

	pipe(fd);
	dup2(fd[0], 0);
	write(fd[1], buff->txt, buff->strlen);
	close(fd[1]);
	close(fd[0]);
}

int			exe_rdr_heredocument(REDIRECT *rdr)
{
	DSTRING		*buff;

	if (!rdr->file)
		return (-1);
	buff = dstr_new(rdr->file);
	exe_rdr_heredoc_pipes(buff);
	dstr_del(&buff);
	return (0);
}

int			exe_heredoc_get_buff(DSTRING **buff, REDIRECT *rdr, ENV *env)
{
	DSTRING	*line;
	DSTRING	*prm;
	int		status;

	line = 0;
	status = 0;
	prm = dstr_new("hered> ");
	while ((line = sh_readline(prm, env)) || 1)
	{
		if (g_intr == 2 && (status = -1))
			break;
		if (!line)
			break ;
		if (ft_strequ(line->txt, rdr->file))
			break ;
		dstr_insert_dstr(*buff, line, (*buff)->strlen);
		dstr_insert_ch(*buff, '\n', (*buff)->strlen);
		dstr_del(&line);
	}
	if (status == -1 && buff)
			dstr_del(buff);
	if (line)
		dstr_del(&line);
	dstr_del((DSTRING **)&prm);
	return (status);
}

int			exe_init_heredoc(REDIRECT *rdr, ENV *env)
{
	DSTRING			*buff;

	if (!(buff = dstr_new("")))
		return (-2);
	if (exe_heredoc_get_buff(&buff, rdr, env))
		return (-1);
	if (rdr->file)
		free(rdr->file);
	if (!(rdr->file = dstr_flush(&buff)))
		return (-2);
	return (0);
}

int			exe_heredoc_check(t_dlist *lst, ENV *envr)
{
	int			err;
	REDIRECT	*rd;

	err = 0;
	while (lst)
	{
		if (!err && (rd = lst->content) && rd->type == herd)
		{
			err = exe_init_heredoc(rd, envr);
		}
		lst = lst->next;
	}
	return (err);
}
