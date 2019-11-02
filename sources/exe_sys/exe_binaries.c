/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_binaries.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:01:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/02 08:54:04 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sys_tools/sys_tools.h"

static void		exe_b_child_alg(EXPRESSION *cmd, char **envp, char *path)
{
	t_dlist		*redrs;
	int			err;

	err = 0;
	redrs = cmd->redirections;
	while (redrs)
	{
		if (!err && (err = exe_redir_ex(redrs->content, 0)))
			sys_error_handler(0, -err, 0);
		redrs = redrs->next;
	}
	if (cmd->ipipe_fds && (dup2(cmd->ipipe_fds[0], 0) >= 0))
	{
		close(cmd->ipipe_fds[1]);
		close(cmd->ipipe_fds[0]);
	}
	if (cmd->opipe_fds && (dup2(cmd->opipe_fds[1], 1) >= 0))
	{
		close(cmd->opipe_fds[1]);
		close(cmd->opipe_fds[0]);
	}
	path == 0 ? path = cmd->args[0] : 0;
	if (!err)
		execve(path, cmd->args, envp);
	exit(126); // TODO: Error HANDLING need for failed execve
}

int				exe_execute_pi(EXPRESSION *cmd, ENV *envr)
{
	pid_t		pid;
	char		**envp;
	char		*path;

	pid = 0;
	cmd->opipe_fds ? pipe(cmd->opipe_fds) : 0;
	if (!(path = sh_checkbins(cmd->args[0], envr, &pid)))
		return (pid); // ERROR: exec: Command not found.
	if (!(envp = ft_avl_tree_to_warr(envr->globals, avln_todstring_key_val)))
		return (-8); // ERROR: exec: Malloc failed
	pid = fork();
	if (pid == 0)
		exe_b_child_alg(cmd, envp, path);
	if (pid < 0)
		return (-2);  // ERROR: exec: Fork failed
	envp ? et_rm_warr(envp) : 0;
	path ? free(path) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[0]) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[1]) : 0;
	return ((int)pid);
}
