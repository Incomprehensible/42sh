/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 17:42:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/17 10:29:00 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"
#include "sh_tokenizer.h"
#include "printf/libftprintf.h"
#include "printf/longnumber.h"
#include "sys_tools/sys_tools.h"

void	skip_alnums_n_space(char *str, size_t *ind)
{
	while (ft_isalnum(str[*ind]) || ft_isspace(str[*ind]))
		++(*ind);
}

long	do_math_bltn(char *str_xp, ENV *envr, ERR *err)
{
	size_t		ind;
	char		*str;
	DSTRING		*expr;
	long		res;

	ind = 0;
	skip_alnums_n_space(str_xp, &ind);
	expr = dstr_new(str_xp);
	res = ariphmetic_eval(expr->txt, envr, err);
	dstr_del(&expr);
	return (res);
}

int		bltn_math(char **args, ENV *env)
{
	DSTRING		*expr;
	ERR			err;
	long		res;

	// TODO: Need to do malloc safer.
	err.err_code = 0;
	err.error_msg = NULL;
	res = do_math_bltn(args[1], env, &err);
	if (err.err_code)
		math_error_handler(&err, 0, env);
	res = res == 0 ? EXIT_FAILURE : EXIT_SUCCESS;
	return ((int)res);
}
