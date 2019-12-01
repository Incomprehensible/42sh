/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_binaries.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:01:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/01 18:49:10 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "parser.h"
#include "sys_tools/sys_tools.h"

static void		exe_b_child_alg(EXPRESSION *cmd, char **envp, char *path, \
ENV *env)
{
	int			err;

	sys_sig_dfl();
	close(g_prompt_fd);
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
	if ((err = exe_redir_list(cmd->redirections, env)))
		exit(127);
	execve(path, cmd->args, envp);
	exit(126);
}

int				exe_xpr_cleanup(char *path, int err_code)
{
	if (path)
		free(path);
	return (err_code);
}

int				exe_execute_pi(EXPRESSION *cmd, ENV *envr)
{
	pid_t		pid;
	char		**envp;
	char		*path;

	pid = -1;
	cmd->opipe_fds ? pipe(cmd->opipe_fds) : 0;
	if (!(path = sh_checkbins(cmd->args[0], envr, &pid)))
		return (exe_xpr_cleanup(path, -1));
	if ((exe_heredoc_check(cmd->redirections, envr)))
		return (exe_xpr_cleanup(path, 0));
	if (!(envp = ft_avl_tree_to_warr(envr->globals, avln_todstring_key_val)))
		return (exe_xpr_cleanup(path, -8));
	pid = fork();
	if (pid == 0)
		exe_b_child_alg(cmd, envp, path, envr);
	cmd->ipipe_fds ? close(cmd->ipipe_fds[0]) : 0;
	cmd->ipipe_fds ? close(cmd->ipipe_fds[1]) : 0;
	if (pid < 0)
		return (-2);
	envp ? et_rm_warr(envp) : 0;
	path ? free(path) : 0;
	return ((int)pid);
}
