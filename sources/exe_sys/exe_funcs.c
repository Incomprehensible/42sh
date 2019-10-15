/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 17:19:55 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 08:56:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "parser.h"
#include "sh_token.h"
#include "sys_tools/sys_tools.h"

t_avl_tree	*func_core_create(EXPRESSION *expr)
{
	t_avl_tree		*cores;
	ssize_t			i;
	char			*key;
	char			*value;

	i = 0;
	if (!(cores = ft_avl_tree_create(free)))
		return (0);
	while (expr->args[i])
	{
		if (!(key = ft_lltoa_base(i, 10)))
			return (0);
		env_core_set(key, expr->args[i], cores); //! TODO: CHECK FOR MALLOC ERRORS
		free(key);
		++i;
	}
	if (!(key = ft_lltoa_base(--i, 10)))
			return (0);
	env_core_set("#", key, cores);
	free(key);
	return (cores);
}

int			exe_b_func_alg(EXPRESSION *expr, ENV *envr, FUNC *func)
{
	t_dlist		*redirs;
	int			status;
	t_avl_tree	*tmp;
	int			err;

	err = 0;
	redirs = expr->redirections;
	while (redirs)
	{
		if (!err && (err = exe_redir_ex(redirs->content, envr)))
			sys_error_handler(0, -err, envr);
		redirs = redirs->next;
	}
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
	if (err)
		exit(2);
	tmp = envr->core;
	envr->core = func_core_create(expr);
	sh_tparse(func->func_code, envr, TK_FEND, &status);
	ft_avl_tree_free(envr->core);
	envr->core = tmp;
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
	t_avl_tree	*tmp;
	int			err;

	err = 0;
	exe_redir_save420(expr->redirections);
	redirs = expr->redirections;
	while (redirs)
	{
		if (!err && (err = exe_redir_ex(redirs->content, envr)))
			break ;
		redirs = redirs->next;
	}
	//safe special params and args of global scope and create new
	if (!err)
	{
		tmp = envr->core;
		envr->core = func_core_create(expr);
		sh_tparse(func->func_code, envr, TK_FEND, status);
		ft_avl_tree_free(envr->core);
		envr->core = tmp;
	}
	//load special params and args of GS and delete locals of function
	exe_redir_load420(expr->redirections);
	return (err);
}
