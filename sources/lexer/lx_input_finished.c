/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_finished.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/30 06:58:54 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	func_really_closed(char *str)
{
	short	times;

	str = skip_spaces_nd_nl(str);
	if (!(*str))
		return (0);
	if (*str == '{')
	{
		times = br_closed(str, '{', '}');
		if (times)
		{
			parse_err = times < 0 ? PRO_NONE : parse_err;
			return (times > 0 ? 0 : -1);
		}
		return (1);
	}
	if (*str)
		return (1);
	return (0);
}

short	func_is_closed(char *str)
{
	size_t	i;

	str = skip_spaces(str);
	if (!*str)
		return (1);
	if ((i = layer_parse_two("? ", str)))
		return (func_really_closed(str + i));
	return (1);
}

short	funcs_closed(char *str)
{
	ssize_t	i;

	str = skip_spaces(str);
	while (*str)
	{
		if (is_token_here(str, "function"))
			return (func_is_closed(str + 8));
		else if ((i = layer_parse_two("?()_", str)))
			return (func_really_closed(str + i));
		else if (*str == '{')
		{
			if (!(i = func_really_closed(str)))
				return (2);
			parse_err = i < 0 ? PRO_NONE : parse_err;
			return (i);
		}
		str += (*str == '\\') ? mirror_passes(str) : 1;
	}
	return (1);
}

short	parse_error(void)
{
	if (parse_err == PRO_SUBSH)
		ft_putstr("42sh: parse error in subshell: unexpected ')'");
	else if (parse_err == PRO_SQU)
		ft_putstr("42sh: parse error: '[' didn't close");
	else if (parse_err == PRO_NONE)
		ft_putstr("42sh: parse error in function: unexpected '}'");
	else
		ft_putstr("42sh: parse error occured");
	parse_err = 1;
	return (-1);
}

short	input_finished(char *str)
{
	short	id;

	if (!input_closed(str))
		return (0);
	id = brackets_closed(str);
	if (id != 1)
		return (id ? parse_error() : 0);
	id = quotes_closed(str);
	if (id != 1)
		return (id ? parse_error() : 0);
	if (!scripts_closed(str))
	{
		input_not_over = PRO_NONE;
		return (0);
	}
	id = funcs_closed(str);
	if (id != 1)
	{
		input_not_over = id ? input_not_over : PRO_NONE;
		input_not_over = id == 2 ? PRO_LAM : input_not_over;
		id = id == 2 ? 0 : id;
		return (id ? parse_error() : 0);
	}
	return (1);
}
