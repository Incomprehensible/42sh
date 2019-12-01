/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 03:32:33 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	q_closed(char *str, char q, char q1, char q2)
{
	size_t	jump;
	short	times;

	times = 0;
	while (*str)
	{
		if (*str == '\\')
			str += mirror_passes(str);
		else
		{
			if (*str == q)
				times = (times) ? --times : ++times;
			if (!times && (*str == q1 || *str == q2))
			{
				if (!(jump = skip_field(str, *str)))
					return (0);
				str += jump;
			}
			else if (*str == '(')
				str += skip_field(str, *str);
			str++;
		}
	}
	return (times ? 0 : 1);
}

char	*br_ap_cls_n(char *str)
{
	while (*str)
	{
		if (*str == '\'')
			return (++str);
		if (*str++ == '\\' && *str == '\'')
			++str;
	}
	return (str);
}

char	*br_cls_n(char *str, short *i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"')
			str = br_qt_cls_n(++str, i);
		else if (*str == '(' && (++(*i) || 1))
			str = br_cls_n(++str, i);
		else if (*str == '\'')
			str = br_ap_cls_n(++str);
		else if (*str == ')' && (--(*i) || 1))
			return (++str);
		else
			++str;
	}
	return (str);
}

char	*br_qt_cls_n(char *str, short *i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"')
			return (++str);
		else if (*str == '$' && *(++str) == '(' && (++(*i) || 1))
			str = br_cls_n(++str, i);
		else
			++str;
	}
	return (str);
}
