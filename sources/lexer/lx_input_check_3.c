/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/17 19:20:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short			br_closed_n(char *str, short i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\'' || *str == '\"' || *str == '(' || *str == ')')
			str = br_cls_n(str, &i);
		else
			++str;
	}
	return (i);
}

short			brackets_closed(char *str)
{
	short	times;
	short	math_flag;

	times = br_closed_n(str, 0);
	math_flag = 0;
	if (layer_parse_one("((", str) || layer_parse_one("~((", str))
		math_flag = 1;
	if (times)
	{
		g_parse_err = times < 0 ? PRO_SUBSH : g_parse_err;
		g_input_nover = times > 0 ? PRO_SUBSH : g_input_nover;
		g_input_nover = (times > 0 && math_flag) ? PRO_MATH : g_input_nover;
		return (times > 0 ? 0 : -1);
	}
	return (1);
}

static short	is_and_closed(char *str, size_t size)
{
	if (*(str + size - 1) == '&' && (size - 2) && *(str + size - 2) == '&')
	{
		if ((size - 3) && *(str + size - 2) == '\\')
			return (1);
		return (0);
	}
	return (1);
}

short			input_closed(char *str)
{
	int	size;

	size = ft_strlen(str);
	while (size && (*(str + size - 1) == ' ' || *(str + size - 1) == '\t'))
		--size;
	if (size && *(str + size - 1) == '|' && (size - 2 >= 0) &&
	*(str + size - 2) != '\\')
		return (parse_err_check(str, size));
	else if (size && *(str + size - 1) == '\\')
	{
		if (size == 1 || !check_input_mir(str, '\\'))
			g_input_nover = PRO_MIRR;
	}
	else if (size && !is_and_closed(str, size))
	{
		if (check_input_seq(str, '&'))
			g_input_nover = PRO_AND;
		else
		{
			g_parse_err = 1;
			return (-1);
		}
	}
	return (g_input_nover == -1 ? 1 : 0);
}

short			are_tokens_here(char *str)
{
	if (is_token_here(str, "while"))
		return (1);
	if (is_token_here(str, "for"))
		return (1);
	if (is_token_here(str, "until"))
		return (1);
	return (0);
}
