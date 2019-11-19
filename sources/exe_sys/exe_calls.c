/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:03:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 12:04:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int			exe_execute_expr(EXPRESSION *expr, ENV *envr, int *status)
{
	int		(*bltn)(char **, ENV *envr);
	t_func	*func;
	t_avln	*f_node;

	if (!expr->args[0] && !(*status = 0))
		return (0);
	if ((f_node = ft_avl_search(envr->builtns, expr->args[0])))
	{
		bltn = f_node->content;
		if (expr->ipipe_fds || expr->opipe_fds)
			return (exe_execute_pi_b(expr, envr, bltn));
		else
			return (exe_execute_b(expr, envr, bltn, status));
	}
	else if ((f_node = ft_avl_search(envr->funcs, expr->args[0])))
	{
		func = f_node->content;
		if (expr->ipipe_fds || expr->opipe_fds)
			return (exe_execute_pi_f(expr, envr, func));
		else
			return (exe_execute_f(expr, envr, func, status));
	}
	return (exe_execute_pi(expr, envr));
}
