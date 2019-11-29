/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 18:13:05 by bomanyte         ###   ########.fr       */
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

	times = br_closed_n(str, 0);
	if (times)
	{
		PARSE_ERR = times < 0 ? PRO_SUBSH : PARSE_ERR;
		INPUT_NOT_OVER = times > 0 ? PRO_SUBSH : INPUT_NOT_OVER;
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
	if (*(str + size - 1) == '|' && (size - 2 >= 0) &&
	*(str + size - 2) != '\\')
	{
		INPUT_NOT_OVER = *(str + size - 2) == '|' ? PRO_OR : PRO_PIPE;
		return (0);
	}
	if (*(str + size - 1) == '\\' && (size - 2 >= 0) &&
	*(str + size - 2) != '\\')
	{
		INPUT_NOT_OVER = PRO_NONE;
		return (0);
	}
	if (!is_and_closed(str, size))
	{
		INPUT_NOT_OVER = PRO_AND;
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
