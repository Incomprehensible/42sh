/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops_dec_inc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:24:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 01:18:43 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

// TODO: MALLOC CHECKS
long	dec_inc_do(DSTRING *ex, ENV *envr, int is_dec)
{
	size_t	ind;
	long	res;
	DSTRING	*dstr;
	char	*new_val;
	char	*name;

	res = 0;
	ind = 0;
	m_skip_spaces(ex->txt, &ind);
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
	m_skip_spaces(ex->txt, &ind);
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
	m_skip_spaces(ex->txt, &ind);
	if (ft_isalpha(ex->txt[ind]))
		return (dec_inc_do(ex, envr, 1));
	res = -1 + math_eval(ex, envr);
	dstr_del(&ex);
	return (res);
}
