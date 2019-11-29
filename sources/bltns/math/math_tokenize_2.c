/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tokenize_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 02:05:31 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

char		*pull_predessor(char *expr, t_dlist **math, ERR *err)
{
	t_tk_type	type;
	char		op;
	int			n;

	op = *expr++;
	err = 0;
	n = 1;
	while (op != '!' && *expr == op && n <= 1)
	{
		expr++;
		n++;
	}
	type = (op == '!') ? REJECT : 0;
	type = (op == '+' && n == 1) ? POSIT : type;
	type = (op == '+' && n == 2) ? INCRM : type;
	type = (op == '-' && n == 1) ? NEGAT : type;
	type = (op == '-' && n == 2) ? DECRM : type;
	make_token(math, NULL, type);
	return (expr);
}

char		*pull_operator(char *expr, t_dlist **math)
{
	t_tk_type	type;
	t_tk_type	swap;

	type = (*expr == '!') ? REJECT : 0;
	type = (*expr == '=') ? EQU : type;
	type = (*expr == '-') ? MINUS : type;
	type = (*expr == '+') ? PLUS : type;
	type = (*expr == '*') ? MULT : type;
	type = (*expr == '/') ? DEVID : type;
	type = (*expr == '%') ? DEVREM : type;
	type = (*expr == '&') ? AND : type;
	type = (*expr == '|') ? OR : type;
	type = (*expr == '^') ? XOR : type;
	type = (*expr == '~') ? NOT : type;
	swap = type;
	type = (type == REJECT && *(expr + 1) == '=') ? MIN_EQ : type;
	type = (type == EQU && *(expr + 1) == '=') ? IS_EQU : type;
	type = (type == MINUS && *(expr + 1) == '=') ? MIN_EQ : type;
	type = (type == PLUS && *(expr + 1) == '=') ? PLUS_EQ : type;
	type = (type == AND && *(expr + 1) == '&') ? LOG_AND : type;
	type = (type == OR && *(expr + 1) == '|') ? LOG_OR : type;
	make_token(math, NULL, type);
	return (swap == type ? expr + 1 : expr + 2);
}

t_tk_type	get_hex_or_bin(t_dlist **math, t_mtx **bases, char *operand)
{
	if (parse_base(bases[0]->fin_meta, operand))
	{
		make_token(math, ft_strdup(operand + 2), bases[0]->base);
		free(operand);
		return (BIN);
	}
	else if (parse_base(bases[1]->fin_meta, operand))
	{
		make_token(math, ft_strdup(operand + 2), bases[1]->base);
		free(operand);
		return (HEX);
	}
	return (0);
}

char		*get_operand(char *expr)
{
	size_t	i;
	char	*operand;

	i = 0;
	while (expr[i] && !(is_op(expr[i])))
		i++;
	operand = pull_token(expr, i);
	return (operand);
}

char		*pull_number(char *expr, t_dlist **math, ERR *err)
{
	static t_mtx	*bases[5];
	char			*operand;

	if (!bases[0])
		init_num_bases(bases);
	operand = get_operand(expr);
	expr += ft_strlen(operand);
	if (layer_parse_two(bases[0]->strt_meta, operand))
	{
		if (get_hex_or_bin(math, bases, operand))
			return (expr);
		return (set_error(operand, VALUE_TOO_GREAT, err));
	}
	if (!get_base(operand, bases, math))
		return (set_error(operand, VALUE_TOO_GREAT, err));
	return (expr);
}
