/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 02:24:57 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

static char		*pull_math_tok(short i, char *expr, t_dlist **math, ERR *err)
{
	expr = i == 1 ? pull_bit_offset(expr, math, err) : expr;
	expr = i == 2 ? pull_math_compr(expr, math, err) : expr;
	expr = i == 3 ? pull_predessor(expr, math, err) : expr;
	expr = i == 4 ? pull_operator(expr, math) : expr;
	return (expr);
}

static short	get_op_id(char *expr, short flag)
{
	short	id;

	id = 0;
	if ((*expr == '>' || *expr == '<') &&
	(*(expr + 1) == '>' || *(expr + 1) == '<'))
		return (1);
	else if (*expr == '>' || *expr == '<')
		id = 2;
	else if (!flag && (*expr == '!' || *expr == '+' || *expr == '-'))
		id = 3;
	else if (is_op(*expr))
		id = 4;
	return (id);
}

static char		*ariphmetic_tokenize(char *expr, t_dlist **math, ERR *err)
{
	short	flag;
	short	ind;

	flag = 0;
	expr = skip_spaces(expr);
	while (expr && *expr)
	{
		if ((ind = get_op_id(expr, flag)))
		{
			expr = pull_math_tok(ind, expr, math, err);
			flag = 0;
		}
		else if (*expr == '(' || *expr == ')')
			expr = pull_bracket(expr, math);
		else
		{
			expr = pull_operand(expr, math, err);
			flag = 1;
		}
		expr = expr ? skip_spaces(expr) : expr;
	}
	return (expr);
}

static long		get_math_result(char *expr, ENV *env, ERR *err)
{
	t_dlist	*polish_notation[2];
	t_dlist	*current;

	ft_bzero(polish_notation, sizeof(t_dlist *) * 2);
	polish_notation[0] = ft_dlstnew(NULL, 0);
	polish_notation[1] = polish_notation[0];
	current = polish_notation[0];
	current->content = NULL;
	current->next = NULL;
	while (*expr)
	{
		if (!(expr = ariphmetic_tokenize(expr, polish_notation, err)))
		{
			if (polish_notation[0])
				clear_tokens(polish_notation, 0);
			return (0);
		}
	}
	make_token(polish_notation, NULL, TK_EOF);
	return (ariphmetic_calc(polish_notation, env, err));
}

long			ariphmetic_eval(char *expr, ENV *env, ERR *err)
{
	char	**exprs;
	long	res;
	int		i;

	res = 0;
	i = 0;
	err->err_code = 0;
	err->error_msg = NULL;
	if (!expr)
		return (0);
	if (ft_strchr(expr, ','))
	{
		exprs = ft_strsplit(expr, ',');
		while (exprs[i] && !err->err_code)
		{
			res = get_math_result(exprs[i], env, err);
			i++;
		}
		ft_arrmemdel((void **)exprs);
	}
	else
		res = get_math_result(expr, env, err);
	return (res);
}
