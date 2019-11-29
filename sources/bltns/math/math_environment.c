/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 17:21:27 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

char	*env_set(char *new, long res, ENV *env)
{
	char			*to_str;
	t_dyn_string	*value;

	to_str = ft_itoa(res);
	value = dstr_new(to_str);
	env_set_variable(new, value, env);
	dstr_del(&value);
	return (to_str);
}

char	*pull_env_val(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char			*new;
	t_dyn_string	*value;
	t_tk_type		type;
	long			res;

	new = ((t_tok *)opd_stack->content)->value;
	value = env_get_variable(((t_tok *)opd_stack->content)->value, env);
	if (!(*(value->txt)))
	{
		free(value->txt);
		value->txt = ft_strdup("0");
	}
	if (!(type = is_number(value->txt)))
		return (error_process(new, value, STR_OPERAND, err));
	res = ft_atoi_base(value->txt, type);
	new = ((t_tok *)opd_stack->prev->content)->value;
	if (((t_tok *)opd_stack->prev->content)->type != OPRND)
		return (error_process(new, value, INVALID_ASSIG, err));
	dstr_del(&value);
	return (env_set(new, res, env));
}

t_dlist	*set_new_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char	*value;

	if (((t_tok *)opd_stack->content)->type == OPRND)
		value = pull_env_val(opd_stack, env, err);
	else
		value = set_var(opd_stack, env, err);
	opd_stack = opd_stack->prev;
	del_tokens(opd_stack->next);
	free(((t_tok *)opd_stack->content)->value);
	((t_tok *)opd_stack->content)->value = value;
	((t_tok *)opd_stack->content)->type = DEC;
	opd_stack->next = NULL;
	while (opd_stack->prev)
		opd_stack = opd_stack->prev;
	return (opd_stack);
}

char	*set_var(t_dlist *opd_stack, ENV *env, ERR *err)
{
	char		*value;
	t_tk_type	type;
	long		res;

	err = 0;
	value = ((t_tok *)opd_stack->content)->value;
	type = ((t_tok *)opd_stack->content)->type;
	res = ft_atoi_base(value, type);
	value = ((t_tok *)opd_stack->prev->content)->value;
	return (env_set(value, res, env));
}
