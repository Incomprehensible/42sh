/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 17:42:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/08 20:47:07 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bltn.h"
#include "ft_ctype.h"

// static const char	*ops[] = {
// 	"><", "&&", "||",
// 	">", "<", ">=", "<=",
// 	"+", "-",
// 	"%", "/", "*",
// 	"++", "--", NULL
// };

long	math_eval(DSTRING *expr, ENV *envr);

int		skip_brackets(char *str, size_t *ind)
{
	int		br_cnt;

	br_cnt = 0;
	while (1)
	{
		if (str[*ind] == 0)
			return (0);
		else if (str[*ind] == '(')
			++br_cnt;
		else if (str[*ind] == ')')
			--br_cnt;
		++(*ind);
		if (!br_cnt)
			return (1);
	}
}

void	skip_word(char *str, size_t *ind)
{
	while (ft_isalnum(str[*ind]))
		++(*ind);
}

void	skip_alnums_n_space(char *str, size_t *ind)
{
	while (ft_isalnum(str[*ind]) || ft_isspace(str[*ind]))
		++(*ind);
}

void	skip_spaces(char *str, size_t *ind)
{
	while (ft_isspace(str[*ind]))
		++(*ind);
}

DSTRING	*get_res_var(long res)
{
	DSTRING		*resd;
	char		*str;

	str = ft_lltoa_base(res, 10);
	resd = dstr_new(str);
	free(str);
	return (resd);
}

char	*get_name_var(char *str_xp)
{
	size_t	ind;
	char	*str;
	size_t	subs[2];

	ind = 0;
	skip_spaces(str_xp, &ind);
	subs[0] = ind;
	skip_word(str_xp, &ind);
	subs[1] = ind - subs[0];
	str = ft_strsub(str_xp, subs[0], subs[1]);
	return (str);
}

long	mop_add(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) + math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mlt(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) * math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_div(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) / math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_suj(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) - math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mod(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) % math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_eq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) == math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_neq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) != math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_lw(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) < math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_bg(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) > math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_lweq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) <= math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}


long	mop_bgeq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) >= math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_or(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) || math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_and(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) && math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

// TODO: Malloc error checking.
long	dec_inc_do(DSTRING *ex, ENV *envr, int is_dec)
{
	size_t	ind;
	long	res;
	DSTRING	*dstr;
	char	*new_val;
	char	*name;

	res = 0;
	ind = 0;
	skip_spaces(ex->txt, &ind);
	name = get_name_var(ex->txt);
	dstr = env_get_variable(name, envr);
	res = ft_atoll_base(dstr->txt, 10);
	res += is_dec ? -1 : 1;
	new_val = ft_lltoa_base(res, 10);
	dstr_del(&dstr);
	dstr = dstr_new(new_val);
	free(new_val);
	env_set_variable(name, dstr, envr);
	free(name);
	dstr_del(&dstr);
	dstr_del(&ex);
	return (res);
}

long	mop_inc(DSTRING *ex, ENV *envr)
{
	long	res;
	size_t	ind;

	ind = 0;
	skip_spaces(ex->txt, &ind);
	if (ft_isalpha(ex->txt[ind]))
		return (dec_inc_do(ex, envr, 0));
	res = 1 + math_eval(ex, envr);
	dstr_del(&ex);
	return (res);
}

long	mop_dec(DSTRING *ex, ENV *envr)
{
	long	res;
	size_t	ind;

	ind = 0;
	skip_spaces(ex->txt, &ind);
	if (ft_isalpha(ex->txt[ind]))
		return (dec_inc_do(ex, envr, 1));
	res = -1 + math_eval(ex, envr);
	dstr_del(&ex);
	return (res);
}

size_t	op_search(DSTRING *expr, char *op, size_t op_len)
{
	size_t		ind;
	char		*stri;
	int			i;

	ind = 0;
	stri = expr->txt;
	while (ind < expr->strlen)
	{
		skip_alnums_n_space(stri, &ind);
		if (stri[ind] == '(' && !skip_brackets(stri, &ind))
			return (0);
		if (stri[ind] == ')')
			return (0);
		if (ft_strnequ(&(stri[ind]), op, op_len))
			return (ind);
		++ind;
	}
	return (SIZE_T_MAX);
}

long	value_ret(char *str, ENV *envr)
{
	long	res;
	DSTRING	*var;

	var = 0;
	if (*str == '$')
	{
		var = env_get_variable(str + 1, envr);
		str = var->txt;
	}
	else if (ft_isalpha(*str))
	{
		var = env_get_variable(str, envr);
		str = var->txt;
	}
	res = ft_atoll_base(str, 10);
	if (var)
		dstr_del(&var);
	return (res);
}

long	expr_ret(DSTRING *expr, ENV *envr)
{
	char	*trimmed;
	DSTRING	*brs_xpr;
	long	res;

	trimmed = ft_strtrim(expr->txt);
	if (trimmed[0] == '(')
	{
		trimmed[ft_strlen(trimmed) - 1] = ' ';
		trimmed[0] = ' ';
		brs_xpr = dstr_new(trimmed);
		res = math_eval(brs_xpr, envr);
		dstr_del(&brs_xpr);
	}
	else
		res = value_ret(trimmed, envr);
	free(trimmed);
	return (res);
}
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
