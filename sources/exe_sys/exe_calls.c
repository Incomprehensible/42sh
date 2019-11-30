/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:03:40 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/30 06:58:15 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

int			subsh_tok_err(char *contx, int rcode, t_dlist **tks, ENV *envr)
{
	extern char		*g_last_input;

	ft_dlst_delf(tks, 0, free_token);
	ft_strdel(&g_last_input);
	input_not_over = -1;
	g_hsh = 0;
	return (sys_perror(contx, rcode, envr));
}

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
