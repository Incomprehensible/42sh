/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_checkers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/08 21:06:34 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	valid_deref(char *str, size_t j)
{
	if (*str == '$' && *(str + 1) != '=' && !is_separator(*(str + 1)))
		return (1);
	if (!j && *str == '~' && (is_separator(*(str + 1)) || *(str + 1) == '/'))
		return (1);
	return (0);
}

short	is_ariphmetic(char str)
{
	if (str == '<' || str == '>' || str == '=')
		return (1);
	return (0);
}

short	is_separator(char str)
{
	if (str != ' ' && str != '\t' && str != '\n' && str != ';'
	&& str != '&' && str != '|' && str != '"' && str)
		return (0);
	return (1);
}

short	is_assign(char c)
{
	if (c != '=' && c != '+' && c != '-')
		return (0);
	return (1);
}

short	is_it_q(char c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}
