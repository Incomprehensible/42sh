/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ret_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:22:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 01:18:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

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
	m_skip_spaces(str_xp, &ind);
	subs[0] = ind;
	skip_word(str_xp, &ind);
	subs[1] = ind - subs[0];
	str = ft_strsub(str_xp, subs[0], subs[1]);
	return (str);
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
