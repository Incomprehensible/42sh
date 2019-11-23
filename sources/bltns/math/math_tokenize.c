/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 01:10:53 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

void	*set_error(char *err_token, int code, ERR *err)
{
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
	type = (op == '>') ? MORE : type;
	if (*expr == '<' || *expr == '>')
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

size_t get_base_seq(char *str, char *meta)
{
    while (*str)
    {
		if (!got_in_seq(*str, meta + 1))
			return (0);
        str++;
    }
    return (1);
}

size_t parse_base(char *meta, char *str)
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

static t_tk_type	get_hex_or_bin(t_dlist **math, t_mtx **bases, char *operand)
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

static t_tk_type	get_base(char *op, t_mtx **bases, t_dlist **math)
{
	t_mtx			*base;
	size_t			i;

	i = 2;
	while (bases[i])
	{
		base = bases[i];
		while (base)
		{
			if (layer_parse_two(base->strt_meta, op))
			{
				if (!(parse_base(base->fin_meta, op)))
					return (0);
				if (base->base == SEV)
				{
					if (base->next)
						make_token(math, ft_strdup(op + 2), base->base);
					else
						make_token(math, ft_strdup(op + 1), base->base);
					free(op);
				}
				else
					make_token(math, op, base->base);
				return (base->base);
			}
			base = base->next;
		}
		i++;
	}
	return (0);
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
		return (set_error(operand, VALUE_TOO_GREAT,err));
	}
	if (!get_base(operand, bases, math))
		return (set_error(operand, VALUE_TOO_GREAT, err));
	return (expr);
}

static char	*pull_operand(char *expr, t_dlist **math, ERR *err)
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

static char	*ariphmetic_tokenize(char *expr, t_dlist **math, ERR *err)
{
	short		flag;

	flag = 0;
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

static long get_math_result(char *expr, ENV *env, ERR *err)
{
	t_dlist		*polish_notation[2];
	t_dlist		*current;

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

long		ariphmetic_eval(char *expr, ENV *env, ERR *err)
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