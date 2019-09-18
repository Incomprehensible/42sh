/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_subsh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:03:04 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/18 21:30:45 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_tools.h"
#include "ft_lbuffer.h"

/*
!! TEMPORARY FUNCTION
!! Soon will be changed!
*/
char			*get_deref_subsh(char *code, ENV *envr)
{
	int			pips[2];
	pid_t		pid;

	pipe(pips);
	pid = fork();
	if (pid == 0)
	{
		close(pips[0]);
		dup2(pips[1], 1);
		close(pips[1]);
		char *argv[4];
		argv[0] = "bash";
		argv[1] = "-c";
		argv[2] = code;
		argv[3] = 0;
		execv("/bin/bash", argv);
		exit(2);
	}
	close(pips[1]);
	t_lbuf *buff = ft_lb_readbytes(pips[0], 0);
	char *res = ft_lb_flush(buff);
	return(res);
}

/*
!! TEMPORARY FUNCTION
!! Soon will be changed!
*/
char			*prc_substitute(char *code, ENV *envr, int is_in)
{
	int			pips[2];
	pid_t		pid;
	DSTRING		*dstr;
	char		*tmp;

	sys_create_pipe(pips);
	pid = fork();
	if (pid == 0)
	{
		if (is_in)
		{
			close(pips[1]);
			dup2(pips[0], 0);
			close(pips[0]);
		}
		else
		{
			close(pips[0]);
			dup2(pips[1], 1);
			close(pips[1]);
		}
		char *argv[4];
		argv[0] = "bash";
		argv[1] = "-c";
		argv[2] = code;
		argv[3] = 0;
		execv("/bin/bash", argv);
		exit(2);
	}
	dstr = dstr_new("/dev/fd/");
	if (is_in)
	{
		sys_destroy_pipe(pips[0]);
		tmp = ft_itoa(pips[1]);
	}
	else
	{
		sys_destroy_pipe(pips[1]);
		tmp = ft_itoa(pips[0]);
	}
	dstr_insert_str(dstr, tmp, MAX_LL);
	free(tmp);
	tmp = ft_strdup(dstr->txt);
	dstr_del(&dstr);
	return (tmp);
}
