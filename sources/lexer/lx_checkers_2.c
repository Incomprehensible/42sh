/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_checkers_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 23:48:16 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short	special_condition(char *patt)
{
	patt = skip_spaces(patt);
	if (*patt == '!')
		return (1);
	return (0);
}

short	mirrored(char *arr)
{
	if (*arr != '"' && *arr != '$' && *arr != '\n')
		return (0);
	return (1);
}

short	is_it_br(char c)
{
	if (c == '(')
		return (1);
	return (0);
}

short	is_q(char c)
{
	if (c != '\'' && c != '"')
		return (0);
	return (1);
}

short	math_ended(t_tk_type type)
{
	if (type == TK_SEP || type == TK_EMPTY)
		return (1);
	return (0);
}
