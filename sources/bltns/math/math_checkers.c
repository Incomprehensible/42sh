/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_checkers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 21:57:33 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

t_tk_type	is_number(char *value)
{
	t_tk_type type;

	if (value && (*value == '-' || *value == '+'))
		value++;
	if (parse_base("0x@01@", value))
		type = BIN;
	else if (parse_base("0x@0123456789abcdefABCDEF@", value))
		type = HEX;
	else if (parse_base("0@01234567@", value))
		type = SEV;
	else if (parse_base("0o@01234567@", value))
		type = SEV;
	else if (!(*value == '0' && *(value + 1)) &&
	parse_base("@0123456789@", value))
		type = DEC;
	else
		return (0);
	return (type);
}

short		stop_token(t_tk_type stop, t_tk_type current)
{
	if (stop != TK_EOF)
		if (current == INTO_BR)
			return (1);
	return (0);
}

short		is_bracket(t_tk_type type)
{
	if (type == INTO_BR || type == OUT_BR)
		return (1);
	return (0);
}

short		is_operand_tok(t_tk_type type)
{
	if (type == OPRND || type == HEX || type == BIN ||
	type == DEC || type == SEV)
		return (1);
	return (0);
}

short		is_op(char op)
{
	char	*ops;

	ops = " -+*/%!~><&^|()=";
	while (*ops)
	{
		if (op == *ops)
			return (1);
		ops++;
	}
	return (0);
}
