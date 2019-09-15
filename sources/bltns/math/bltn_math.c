/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 17:42:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:46:49 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

// static const char	*ops[] = {
// 	"><", "&&", "||",
// 	">", "<", ">=", "<=",
// 	"+", "-",
// 	"%", "/", "*",
// 	"++", "--", NULL
// };




// TODO: Norms must be done
long	math_eval(DSTRING *expr, ENV *envr)
{
	size_t		ind;
	long		res;

	if ((ind = op_search(expr, "==", 2)) != SIZE_T_MAX)
		return (mop_eq(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "!=", 2)) != SIZE_T_MAX)
		return (mop_neq(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "<=", 2)) != SIZE_T_MAX)
		return (mop_lweq(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, ">=", 2)) != SIZE_T_MAX)
		return (mop_bgeq(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "<", 1)) != SIZE_T_MAX)
		return (mop_lw(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, ">", 1)) != SIZE_T_MAX)
		return (mop_bg(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "||", 2)) != SIZE_T_MAX)
		return (mop_or(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "&&", 2)) != SIZE_T_MAX)
		return (mop_and(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "+", 1)) != SIZE_T_MAX && !ft_isalnum(expr->txt[ind + 1]) && expr->txt[ind + 1] != '+')
		return (mop_add(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "-", 1)) != SIZE_T_MAX && !ft_isalnum(expr->txt[ind + 1]) && expr->txt[ind + 1] != '-')
		return (mop_suj(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "/", 1)) != SIZE_T_MAX)
		return (mop_div(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "%", 1)) != SIZE_T_MAX)
		return (mop_mod(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "*", 1)) != SIZE_T_MAX)
		return (mop_mlt(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "++", 2)) != SIZE_T_MAX)
		return (mop_inc(dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	if ((ind = op_search(expr, "--", 2)) != SIZE_T_MAX)
		return (mop_dec(dstr_slice(expr, ind + 2, SSIZE_T_MAX), envr));
	return (expr_ret(expr, envr));
}

long	do_math_bltn(char *str_xp, ENV *envr)
{
	size_t		ind;
	char		*str;
	DSTRING		*expr;
	long		res;

	ind = 0;
	skip_alnums_n_space(str_xp, &ind);
	if (str_xp[ind] == '=' && str_xp[ind + 1] != '=')
	{
		str = &str_xp[ind + 1];
		expr = dstr_new(str);
		res = math_eval(expr, envr);
		dstr_del(&expr);
		str = get_name_var(str_xp);
		expr = get_res_var(res);
		env_set_variable(str, expr, envr);
		dstr_del(&expr);
		free(str);
		return (res);
	}
	expr = dstr_new(str_xp);
	res = math_eval(expr, envr);
	dstr_del(&expr);
	return (res);
}

int		bltn_math(char **args, ENV *env)
{
	char		**exprs;
	DSTRING		*expr;
	size_t		ind;
	int			i;
	long		res;

	exprs = ft_strsplits(args[1], ",");
	// TODO: Need to do malloc safer.
	i = 0;
	while (exprs[i])
		res = do_math_bltn(exprs[i++], env);
	et_rm_warr(exprs);
	res = res == 0 ? EXIT_FAILURE : EXIT_SUCCESS;
	return ((int)res);
}
