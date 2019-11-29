/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 01:49:01 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

void	*error_process(char *new, DSTRING *val, int code, ERR *err)
{
	set_error(ft_strdup(new), code, err);
	dstr_del(&val);
	return (NULL);
}

long	check_result(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char	*value;
	long	res;

	res = 0;
	if (!err->err_code)
	{
		value = opd_stack ? ((t_tok *)opd_stack->content)->value : NULL;
		if (!value || (count_dlist(opd_stack)) != 1)
		{
			set_error(ft_strdup(value), OPERAND_EXP, err);
			return (0);
		}
		if (((t_tok *)opd_stack->content)->type == OPRND)
			res = fetch_operand((t_tok *)opd_stack->content, env, err);
		else
			res = ft_atoi_base(value, ((t_tok *)opd_stack->content)->type);
	}
	del_tokens(opd_stack);
	return (res);
}

t_dlist	*process_opd_err(t_dlist *opd_stack, ERR *err)
{
	char	*val;

	val = ((t_tok *)opd_stack->content)->value;
	if (opd_stack)
	{
		set_error(ft_strdup(val), OPERAND_EXP, err);
		return (opd_stack);
	}
	return (set_error(NULL, OPERAND_EXP, err));
}

void	*set_error(char *err_token, int code, ERR *err)
{
	err->err_code = code;
	err->error_msg = err_token;
	return (NULL);
}
