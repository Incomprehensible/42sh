/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_binaries.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:01:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 07:35:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "parser.h"
#include "sys_tools/sys_tools.h"

static void		exe_b_child_alg(EXPRESSION *cmd, char **envp, char *path)
{
	sys_sig_dfl();
	close(g_prompt_fd);
	signal(SIGPIPE, SIG_IGN);
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
	execve(path, cmd->args, envp);
	exit(126);
}

int				exe_execute_pi(EXPRESSION *cmd, ENV *envr)
{
	pid_t		pid;
	char		**envp;
	char		*path;
	int			err;

	pid = -1;
	cmd->opipe_fds ? pipe(cmd->opipe_fds) : 0;
	if (!(path = sh_checkbins(cmd->args[0], envr, &pid)))
		return (pid);
	if (!(envp = ft_avl_tree_to_warr(envr->globals, avln_todstring_key_val)))
		return (-8);
	if (!(err = exe_redir_list(cmd->redirections, envr)))
		pid = fork();
	if (!err && pid == 0)
		exe_b_child_alg(cmd, envp, path);
	cmd->ipipe_fds ? close(cmd->ipipe_fds[0]) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[1]) : 0;
	if (!err && pid < 0)
		return (-2);
	exe_redir_load420(cmd->redirections);
	envp ? et_rm_warr(envp) : 0;
	path ? free(path) : 0;
	if (err)
		return (err);
	return ((int)pid);
}
