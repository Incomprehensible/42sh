/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_binaries.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:01:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/04 06:17:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

static void		exe_b_child_alg(EXPRESSION *cmd, char **envp, char *path)
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
	path == 0 ? path = cmd->args[0] : 0;
	execve(path, cmd->args, envp);
	exit(-1); // TODO: Error HANDLING need for failed execve
}

int				exe_execute_pi(EXPRESSION *cmd, ENV *envr)
{
	pid_t		pid;
	char		**envp;
	char		*path;

	cmd->opipe_fds ? pipe(cmd->opipe_fds) : 0;
	if (!(path = sh_checkbins(cmd->args[0], envr)))
		return (-2); // ERROR: exec: Command not found.
	if (!(envp = ft_avl_tree_to_warr(envr->globals, avln_todstring_key_val)))
		return (-3); // ERROR: exec: Malloc failed
	pid = fork();
	if (pid == 0)
		exe_b_child_alg(cmd, envp, path);
	if (pid < 0)
		return (-1);  // ERROR: exec: Fork failed
	envp ? et_rm_warr(envp) : 0;
	path ? free(path) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[0]) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[1]) : 0;
	return ((int)pid);
}
