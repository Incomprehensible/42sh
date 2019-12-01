/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 18:38:23 by bomanyte         ###   ########.fr       */
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
	short   math_flag;

	times = br_closed_n(str, 0);
	math_flag = 0;
	if (layer_parse_one("((", str) || layer_parse_one("~((", str))
		math_flag = 1;
	if (times)
	{
		parse_err = times < 0 ? PRO_SUBSH : parse_err;
		input_not_over = times > 0 ? PRO_SUBSH : input_not_over;
		input_not_over = (times > 0 && math_flag) ? PRO_MATH : input_not_over;
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
	if (*(str + size - 1) == '|' && (size - 2 >= 0) &&
	*(str + size - 2) != '\\')
	{
		input_not_over = *(str + size - 2) == '|' ? PRO_OR : PRO_PIPE;
		return (0);
	}
	if (*(str + size - 1) == '\\' && (size - 2 >= 0) &&
	*(str + size - 2) != '\\')
	{
		input_not_over = PRO_NONE;
		return (0);
	}
	if (!is_and_closed(str, size))
	{
		input_not_over = PRO_AND;
		return (0);
	}
	return (1);
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
