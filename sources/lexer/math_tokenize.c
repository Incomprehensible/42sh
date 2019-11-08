/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/08 00:32:31 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

//разрезание на выражения
//каждое выражение проходит через валидацию и счет
//приходит строка, чтобы понять, произошла ошибка или нет
//котор конвертится в лонг

void	*set_error(char *err_token, int code, ERR *err)
{
	//error value too great for base
	err->err_code = code;
	err->error_msg = err_token;
	return (NULL);
}

static short	is_op(char op)
{
	char *ops;

	ops = " -+*/%!~><&^|()=";
	while (*ops)
	{
		if (op == *ops)
			return (1);
		ops++;
	}
	return (0);
}

static char	*pull_bit_offset(char *expr, t_dlist **math, ERR *err)
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

static char	*pull_math_compr(char *expr, t_dlist **math, ERR *err)
{
	t_tk_type	type;
	char		op;

	op = *expr++;
	type = (op == '<') ? LESS : 0;
	type = (op == '>') ? MORE : 0;
	if (op == '<' || op == '>')
		return (set_error(NULL, DOUBLE_COMPARE, err));
	type = (*expr == '=' && type == LESS) ? LESS_EQ : type;
	type = (*expr == '=' && type == MORE) ? MORE_EQ : type;
	make_token(math, NULL, type);
	return ((type == MORE || type == LESS || !(*expr)) ? expr : ++expr);
}

static char	*pull_predessor(char *expr, t_dlist **math, ERR *err)
{
	t_tk_type	type;
	char		op;
	int			n;

	op = *expr++;
	n = 1;
	while (*expr == op)
	{
		expr++;
		n++;
	}
	if (op == '!' && n > 1)
		return (set_error(NULL, DOUBLE_NEGATION, err));
	else if ((op == '+' || op == '-') && n > 2)
		return (set_error(NULL, INVALID_INFIX, err));
	type = (op == '!') ? REJECT : 0;
	type = (op == '+' && n == 1) ? POSIT : type;
	type = (op == '+' && n == 2) ? INCRM : type;
	type = (op == '-' && n == 1) ? NEGAT : type;
	type = (op == '-' && n == 2) ? DECRM : type;
	make_token(math, NULL, type);
	return (expr);
}

static char	*pull_operator(char *expr, t_dlist **math)
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

static t_tk_type	get_hex_or_bin(t_dlist **math, t_mtx **bases, char *operand)
{
	if (layer_parse_two(bases[0]->fin_meta, operand))
	{
		make_token(math, operand, bases[0]->base);
		return (HEX);
	}
	else if (layer_parse_two(bases[1]->fin_meta, operand))
	{
		make_token(math, operand, bases[1]->base);
		return (BIN);
	}
	return (0);
}

static t_tk_type	get_base(char *op, t_mtx **bases, t_dlist **math)
{
	t_mtx			*base;
	size_t			i;
	t_tk_type		choice;

	i = 2;
	choice = 0;
	while (bases[i] && !choice)
	{
		base = bases[i];
		while (base)
		{
			if (layer_parse_two(base->strt_meta, op))
			{
				if (!(layer_parse_two(base->fin_meta, op)))
					return (0);
				choice |= base->base;
				make_token(math, op, base->base);
				break ;
			}
			base = base->next;
		}
		i++;
	}
	return (choice);
}

static char	*get_operand(char *expr)
{
	size_t			i;
	char			*operand;

	i = 0;
	while (expr[i] && !(is_op(expr[i])))
		i++;
	operand = pull_token(expr, i);
	return (operand);
}

static char	*pull_number(char *expr, t_dlist **math, ERR *err)
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
		return (set_error(operand, VALUE_TOO_GREAT ,err));
	}
	if (!get_base(operand, bases, math))
		return (set_error(operand, VALUE_TOO_GREAT,err));
	return (expr);
}

static char	*pull_operand(char *expr, t_dlist **math, ERR *err)
{
	char			*operand;

	if (ft_isdigit(*expr))
		return (pull_number(expr, math, err));
	operand = get_operand(expr);
	make_token(math, operand, OPRND);
	expr += ft_strlen(operand);
	return (expr);
}

static char	*pull_bracket(char *expr, t_dlist **math)
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

//double bracket and single bracket to differentiate tokens
static char	*ariphmetic_tokenize(char *expr, ENV *env, t_dlist **math, ERR *err)
{
	t_tk_type	type;
	short		flag;

	flag = 0; //if flag is switched to 1 then we just had an operand
	expr = skip_spaces(expr);
	while (expr && *expr)
	{
		if ((*expr == '>' || *expr == '<') && (*(expr + 1) == '>' || *(expr + 1) == '<'))
		{
			expr = pull_bit_offset(expr, math, err);
			flag = 0;
		}
		else if (*expr == '>' || *expr == '<')
		{
			expr = pull_math_compr(expr, math, err);
			flag = 0;
		}
		else if (*expr == '(' || *expr == ')')
			expr = pull_bracket(expr, math);
		else if (!flag && (*expr == '!' || *expr == '+' || *expr == '-'))
		{
			expr = pull_predessor(expr, math, err);
			flag = 0;
		}
		else if (is_op(*expr))
		{
			expr = pull_operator(expr, math);
			flag = 0;
		}
		else
		{
			expr = pull_operand(expr, math, err);
			flag = 1;
		}
		expr = expr ? skip_spaces(expr) : expr;
	}
	return (expr);
}

//make token list
//send it to calculator
//get str back
//was ariphmetic_tokenize
static long get_math_result(char *expr, ENV *env, ERR *err)
{
	t_dlist		*polish_notation[2];
	t_dlist		*current;

	current = ft_dlstnew(NULL, 0);
	polish_notation[0] = current;
	polish_notation[1] = current;
	current->content = NULL;
	current->next = NULL;
	while (*expr)
	{
		if (!(expr = ariphmetic_tokenize(expr, env, polish_notation, err)))
		{
			if (polish_notation[0])
				clear_tokens(polish_notation, 0);
			return (0);
		}
	}
	make_token(polish_notation, NULL, TK_EOF);
	return (ariphmetic_calc(polish_notation, env, err));
}

// static long	get_math_result(char *expr, ENV *env, ERR *err)
// {
// 	char	*output;
// 	long	res;

// 	if (!(output = ariphmetic_tokenize(expr, env, err)))
// 		return (0);
// 	res = ft_atoi(output);
// 	return (res);
// }

long		ariphmetic_eval(char *expr, ENV *env, ERR *err)
{
	char	*output;
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
			res += get_math_result(exprs[i], env, err);
			i++;
		}
		ft_arrmemdel((void **)exprs);
	}
	else
		res = get_math_result(expr, env, err);
	return (res);
}

