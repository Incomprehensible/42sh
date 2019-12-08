/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_finished.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/08 21:00:40 by hgranule         ###   ########.fr       */
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
			g_parse_err = times < 0 ? PRO_NONE : g_parse_err;
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
			g_parse_err = i < 0 ? PRO_NONE : g_parse_err;
			return (i);
		}
		str += (*str == '\\') ? mirror_passes(str) : 1;
	}
	return (1);
}

short	g_parse_error(void)
{
	if (g_parse_err == PRO_SUBSH)
		sys_perror("parse error in subshell or math: unexpected ')'", 0, 0);
	else if (g_parse_err == PRO_SQU)
		sys_perror("parse error: '[' didn't close", 0, 0);
	else if (g_parse_err == PRO_NONE)
		sys_perror("parse error in function: unexpected '}'", 0, 0);
	else
		sys_perror("parse error occured", 0, 0);
	g_parse_err = 1;
	return (-1);
}

short	input_finished(char *str)
{
	short	id;

	if (!input_closed(str))
		return (0);
	id = brackets_closed(str);
	if (id != 1)
		return (id ? g_parse_error() : 0);
	id = quotes_closed(str);
	if (id != 1)
		return (id ? g_parse_error() : 0);
	if (!scripts_closed(str))
	{
		g_input_nover = PRO_NONE;
		return (0);
	}
	id = funcs_closed(str);
	if (id != 1)
	{
		g_input_nover = id ? g_input_nover : PRO_NONE;
		g_input_nover = id == 2 ? PRO_LAM : g_input_nover;
		id = id == 2 ? 0 : id;
		return (id ? g_parse_error() : 0);
	}
	return (1);
}
