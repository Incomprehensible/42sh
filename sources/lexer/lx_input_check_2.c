/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 18:37:16 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*if_to_the_start(char *str)
{
	str = skip_spaces(str);
	while (*str && !(is_token_here(str, "if")) && !(is_token_here(str, "fi")))
	{
		if (*str == '\\')
			str += mirror_passes(str);
		else if (is_it_q(*str) || is_it_br(*str))
		{
			str += skip_field(str, *str);
			++str;
		}
		else
		{
			while (*str && *str != '\n' && *str != ';')
				str++;
			if (*str)
				str++;
			str = skip_spaces(str);
		}
	}
	return (skip_spaces(str));
}

char	*cycle_to_the_start(char *str)
{
	while (*str && !(are_tokens_here(str)) && !(is_token_here(str, "done")))
	{
		if (*str == '\\')
			str += mirror_passes(str);
		else if (is_it_q(*str) || is_it_br(*str))
		{
			str += skip_field(str, *str);
			++str;
		}
		else
		{
			while (*str && *str != '\n' && *str != ';')
				str++;
			if (*str)
				str++;
			str = skip_spaces(str);
		}
	}
	return (skip_spaces(str));
}

int		validate_ifs(char *str)
{
	int	num;

	num = 0;
	while (*str)
	{
		if ((*str == '\n' || *str == ';') && ++str)
			str = if_to_the_start(str);
		if (*str == '\\')
			str += mirror_passes(str);
		else
		{
			if (is_it_br(*str))
				str += skip_field(str, ')');
			else if (is_it_q(*str))
				str += skip_field(str, *str);
			else if (is_token_here(str, "if"))
				num++;
			else if (is_token_here(str, "fi"))
				num--;
			++str;
		}
	}
	return (num);
}

short	br_closed(char *s, char strt, char fin)
{
	short	times;

	times = 0;
	while (*s)
	{
		if (*s == '\\')
			s += mirror_passes(s);
		else
		{
			if (*s == strt)
				times++;
			if (*s == fin)
				times--;
			if (*s != strt && (is_it_q(*s) || (strt != '(' && *s == '(')))
				s += (*s == '(') ? skip_field(s, ')') : skip_field(s, *s);
			s++;
		}
	}
	return (times);
}

char	*qa_qt_cls_n(char *str, int *q)
{
	short	dump;

	dump = 1;
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"' && (--(*q) || 1))
			return (++str);
		else if (*str == '$' && *(str + 1) == '(')
			str = br_cls_n(++str + 1, &dump);
		else
			++str;
	}
	return (str);
}
