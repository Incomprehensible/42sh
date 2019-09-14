/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 17:19:55 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/14 05:58:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "parser.h"
#include "sh_token.h"

int			exe_b_func_alg(EXPRESSION *expr, ENV *envr, FUNC *func)
{
	t_dlist		*redirs;
	int			status;
	t_avl_tree	*tmp;

	redirs = expr->redirections;
	while (redirs)
	{
		exe_redir_ex(redirs->content);
		redirs = redirs->next;
	}
	if (expr->ipipe_fds && (dup2(expr->ipipe_fds[0], 0) >= 0))
		close(expr->ipipe_fds[1]);
	if (expr->opipe_fds && (dup2(expr->opipe_fds[1], 1) >= 0))
		close(expr->opipe_fds[0]);
	sh_tparse(func->func_code, envr, TK_FEND, &status);
	exit(status);
}

int			exe_execute_pi_f(EXPRESSION *expr, ENV *envr, FUNC *func)
{
	pid_t	pid;

	expr->opipe_fds ? pipe(expr->opipe_fds) : 0;
	pid = fork();
	if (pid == 0)
		exe_b_func_alg(expr, envr, func);
	if (pid < 0)
		return (-1);
	expr->ipipe_fds ? close(expr->ipipe_fds[0]) : 0;
	expr->ipipe_fds ? close(expr->ipipe_fds[1]) : 0;
	return ((int)pid);
}

int			exe_execute_f(EXPRESSION *expr, ENV *envr, FUNC *func, int *status)
{
	t_dlist		*redirs;

	exe_redir_save420(expr->redirections);
	redirs = expr->redirections;
	while (redirs)
	{
		exe_redir_ex(redirs->content);
		redirs = redirs->next;
	}
	//safe special params and args of global scope and create new
	sh_tparse(func->func_code, envr, TK_FEND, status);
	//load special params and args of GS and delete locals of function
	exe_redir_load420(expr->redirections);
	return (0);
}
