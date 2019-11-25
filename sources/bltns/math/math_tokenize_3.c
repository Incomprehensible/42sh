/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tokenize_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 02:32:09 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

size_t	parse_base(char *meta, char *str)
{
	while (*str && *meta && *str == *meta && *meta != '@')
	{
		str++;
		meta++;
	}
	if (*meta != '@')
		return (0);
	if (!get_base_seq(str, meta))
		return (0);
	return (1);
}

char	*pull_bit_offset(char *expr, t_dlist **math, ERR *err)
{
	short	n;
	char	first;
	char	second;

	n = 0;
	first = (expr[n] == '<') ? '<' : '>';
	second = (first == '<') ? '>' : '<';
	while (expr[n] == first)
		n++;
	if (n == 2 && expr[n] != second)
	{
		if (first == '>')
			make_token(math, NULL, BIT_R);
		else
			make_token(math, NULL, BIT_L);
	}
	else
	{
		set_error(pull_token(expr, n), INVALID_OP, err);
		return (NULL);
	}
	return (expr + n);
}

char	*pull_math_compr(char *expr, t_dlist **math, ERR *err)
{
	t_tk_type	type;
	char		op;

	op = *expr++;
	type = (op == '<') ? LESS : 0;
	type = (op == '>') ? MORE : type;
	if (*expr == '<' || *expr == '>')
		return (set_error(NULL, DOUBLE_COMPARE, err));
	type = (*expr == '=' && type == LESS) ? LESS_EQ : type;
	type = (*expr == '=' && type == MORE) ? MORE_EQ : type;
	make_token(math, NULL, type);
	return ((type == MORE || type == LESS || !(*expr)) ? expr : ++expr);
}

char	*pull_operand(char *expr, t_dlist **math, ERR *err)
{
	char			*operand;

	if (ft_isdigit(*expr))
		return (pull_number(expr, math, err));
	expr = (expr && *expr == '$') ? ++expr : expr;
	operand = get_operand(expr);
	make_token(math, operand, OPRND);
	expr += ft_strlen(operand);
	return (expr);
}

char	*pull_bracket(char *expr, t_dlist **math)
{
	while (*expr == '(' || *expr == ')')
	{
		if (*expr == '(')
			make_token(math, NULL, INTO_BR);
		else if (*expr == ')')
			make_token(math, NULL, OUT_BR);
		expr++;
		expr = skip_spaces(expr);
	}
	return (expr);
}
