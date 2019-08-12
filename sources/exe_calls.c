/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:03:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/12 14:28:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_req.h"

static void		exe_b_child_alg(COMMAND *cmd, char **envp)
{
	if (cmd->ipipe_fds && (dup2(cmd->ipipe_fds[0], 0) >= 0))
			close(cmd->ipipe_fds[1]);
		if (cmd->opipe_fds && (dup2(cmd->opipe_fds[1], 1) >= 0))
			close(cmd->opipe_fds[0]);
		execve(cmd->args[0], cmd->args, envp);
		exit(-1); // ERROR
}

int				exe_execute_pi(COMMAND *cmd, char **envp)
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
