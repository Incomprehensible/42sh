/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:00:14 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 22:56:10 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sys_tools/sys_tools.h"

int		exe_b_cbiltn_alg(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *))
{
	signal(SIGPIPE, SIG_IGN);
	close(g_prompt_fd);
	sys_sig_dfl();
	if (expr->ipipe_fds && (dup2(expr->ipipe_fds[0], 0) >= 0))
	{
		close(expr->ipipe_fds[0]);
		close(expr->ipipe_fds[1]);
	}
	if (expr->opipe_fds && (dup2(expr->opipe_fds[1], 1) >= 0))
	{
		close(expr->opipe_fds[0]);
		close(expr->opipe_fds[1]);
	}
	exit(bltn(expr->args, envr));
}

int		exe_execute_pi_b(EXPRESSION *expr, ENV *envr, \
	int (*bltn)(char **, ENV *))
{
	pid_t		pid;
	int			err;

	expr->opipe_fds ? pipe(expr->opipe_fds) : 0;
	if (!(err = exe_redir_list(expr->redirections, envr)))
		pid = fork();
	if (!err && pid == 0)
		exe_b_cbiltn_alg(expr, envr, bltn);
	expr->ipipe_fds ? close(expr->ipipe_fds[0]) : 0;
	expr->ipipe_fds ? close(expr->ipipe_fds[1]) : 0;
	if (!err && pid < 0)
		return (-1);
	exe_redir_load420(expr->redirections);
	return ((int)pid);
}

int		exe_execute_b(EXPRESSION *expr, ENV *envr, \
	int (*bltn)(char **, ENV *), int *status)
{
	t_dlist		*redrs;
	int			err;

	err = 0;
	exe_redir_save420(expr->redirections);
	redrs = expr->redirections;
	while (redrs)
	{
		if (!err && (err = exe_redir_ex(redrs->content, envr)))
			break ;
		redrs = redrs->next;
	}
	if (!err)
		*status = bltn(expr->args, envr);
	else
		*status = 2;
	exe_redir_load420(expr->redirections);
	return (err);
}
