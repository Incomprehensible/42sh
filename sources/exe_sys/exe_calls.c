/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:03:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/14 08:10:21 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_req.h"
#include <stdio.h>

/*
** I NEED TO CHECK FOR ERRORS!!!!!
*/

int				exe_wait_cps(void)
{
	int		status;

	while ((waitpid(-1, &status, WUNTRACED)) || 1)
		if (WIFEXITED(status) || WIFSIGNALED(status))
			break ;
	return (status);
}

void			exe_redir_ex(REDIRECT *rdr)
{
	int			fd;
	int			file_flag;

	fd = rdr->fdr;
	file_flag = O_CREAT;
	rdr->type == r_rdr ? file_flag |= O_RDONLY | O_TRUNC : 0;
	rdr->type == w_rdr ? file_flag |= O_WRONLY | O_TRUNC : 0;
	rdr->type == rw_rdr ? file_flag |= O_RDWR : 0;
	rdr->type == a_rdr ? file_flag |= O_APPEND | O_WRONLY : 0;
	if (rdr->file)
		fd = open(rdr->file, file_flag);
	dup2(fd, rdr->fdl);
}

static void		exe_b_child_alg(EXPRESSION *cmd, char **envp)
{
	t_dlist		*redrs;

	redrs = cmd->redirections;
	while (redrs)
	{
		exe_redir_ex(redrs->content);
		redrs = redrs->next;
	}
	if (cmd->ipipe_fds && (dup2(cmd->ipipe_fds[0], 0) >= 0))
			close(cmd->ipipe_fds[1]);
		if (cmd->opipe_fds && (dup2(cmd->opipe_fds[1], 1) >= 0))
			close(cmd->opipe_fds[0]);
		execve(cmd->args[0], cmd->args, envp);
		exit(-1); // ERROR
}

int				exe_execute_pi(EXPRESSION *cmd, char **envp)
{
	pid_t		pid;

	if (cmd->opipe_fds)
		pipe(cmd->opipe_fds);
	pid = fork();
	if (pid == 0)
		exe_b_child_alg(cmd, envp);
	if (pid < 0) // ERROR
		return (-1);
	cmd->ipipe_fds ? close(cmd->ipipe_fds[0]) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[1]) : 0;
	return ((int)pid);
}
