/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_checkers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/20 23:43:13 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short valid_deref(char *str)
{
	if (*str == '$' && *(str + 1) != '=' && !is_separator(*(str + 1)))
        return (1);
    return (0);
}

short is_ariphmetic(char str)
{
    if (str == '<' || str == '>' || str == '=')
        return (1);
    return (0);
}

short   is_separator(char str)
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

short   is_it_q(char c)
{
    if (c == '\'' || c == '"' || c == '`')
        return (1);
    return (0);
}