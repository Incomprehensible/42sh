/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_validators.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/29 00:03:56 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

size_t			validate_simple_struct(char *s, size_t br)
{
	while (s[br] == '(')
		br++;
	s += br;
	while (ft_isspace(*s))
		s++;
	while (*s && *s != ')')
	{
		if (*s == ';')
			return (0);
		if (*s == '\\')
			s++;
		s++;
	}
	while (*s == ')')
	{
		s++;
		br--;
	}
	return (br ? 0 : 1);
}

size_t			validate_triple_struct(char *s, short pass)
{
	while (s && *s != ')')
	{
		while (ft_isspace(*s))
			s++;
		while (*s && *s != ';' && *s != ')')
		{
			if (*s == '\\')
				s++;
			s++;
		}
		if (!(*s) || (*s != ';' && pass < 2) || (*s != ')' && pass == 2))
			return (0);
		pass = (*s == ';') ? ++pass : pass;
		s++;
	}
	return (pass == 2 ? 1 : 0);
}

static short	slash_tok(t_dlist *token_list)
{
	if (TOK_TYPE == TK_SEP)
	{
		if (TOK_VALUE && *(TOK_VALUE) == '\n')
			return (1);
	}
	return (0);
}

short			check_valid_sep(t_dlist *token_list)
{
	if (!token_list->content || !TOK_TYPE)
		return (0);
	while (token_list && TOK_TYPE != TK_SEP)
		token_list = token_list->prev;
	token_list = (token_list) ? token_list->prev : token_list;
	while (token_list && (TOK_TYPE == TK_EMPTY || slash_tok(token_list)))
		token_list = token_list->prev;
	if (!token_list)
		return (0);
	if (TOK_TYPE != TK_EXPR && TOK_TYPE != TK_NAME && TOK_TYPE != TK_VALUE
	&& TOK_TYPE != TK_SUBSH && TOK_TYPE != TK_MATH && !is_tok_redir(TOK_TYPE, 0)
	&& TOK_TYPE != TK_FI && TOK_TYPE != TK_DONE && TOK_TYPE != TK_BREAK
	&& TOK_TYPE != TK_CONTIN)
		return (0);
	return (1);
}

short			is_prefix(char str)
{
	if (str == '+' || str == '-')
		return (1);
	return (0);
}
