/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 22:00:14 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 20:05:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "sys_tools/sys_tools.h"

int		exe_b_cbiltn_alg(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *))
{
	t_dlist		*redrs;
	int			err;

	err = 0;
	redrs = expr->redirections;
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
	while (redrs)
	{
		if (!err && (err = exe_redir_ex(redrs->content, envr)))
			sys_error_handler(0, -err, envr);
		redrs = redrs->next;
	}
	if (!err)
		exit(bltn(expr->args, envr));
	else
		exit (2);
}

// *RETURNS CPID OF FORKED BLTN BASH
int		exe_execute_pi_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *))
{
	pid_t		pid;

	expr->opipe_fds ? pipe(expr->opipe_fds) : 0;
	pid = fork();
	if (pid == 0)
		exe_b_cbiltn_alg(expr, envr, bltn);
	if (pid < 0)
		return (-1);
	expr->ipipe_fds ? close(expr->ipipe_fds[0]) : 0;
	expr->ipipe_fds ? close(expr->ipipe_fds[1]) : 0;
	return ((int)pid);
}

// *RETURNS ZERO IF OKAY AND MAKE STATUS BY IT SELF
int		exe_execute_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *), int *status)
{
	//* Перенаправление и сохранение потоков
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
	//* запуск функции и сохранение статуса работы в status
	if (!err)
		*status = bltn(expr->args, envr);
	else
		*status = 2;
	//* Восстановление потоков.
	exe_redir_load420(expr->redirections);
	return (err);
}
