/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_rdr_hered.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:17:16 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 17:03:51 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

void		exe_rdr_heredoc_pipes(DSTRING *buff, int *fd)
{
	sys_create_pipe(fd);
	dup2(fd[0], 0);
	write(fd[1], buff->txt, buff->strlen);
	sys_destroy_pipe(fd[0]);
	sys_destroy_pipe(fd[1]);
}

int			exe_heredoc_loop(DSTRING **b, REDIRECT *rdr, ENV *env)
{
	DSTRING			*line;
	DSTRING	const	*prompt = dstr_new("heredoc> ");
	DSTRING			*buff;

	buff = *b;
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
	dstr_del((DSTRING **)&prompt);
	*b = buff;
	return (0);
}

int			exe_rdr_heredocument(REDIRECT *rdr, ENV *env)
{
	DSTRING			*buff;
	int				fd[2];

	buff = dstr_new("");
	if (exe_heredoc_loop(&buff, rdr, env) < 0)
		return (-1);
	exe_rdr_heredoc_pipes(buff, fd);
	dstr_del(&buff);
	return (0);
}
