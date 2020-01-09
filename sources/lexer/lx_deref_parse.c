/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_deref_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/10 22:52:33 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static short	special_reg(char c, short tk)
{
	if (tk == CURLY)
	{
		if (c == '}')
			return (1);
		return (0);
	}
	if (c == '\'' || c == '"' || c == '!' || c == '*' ||
	c == '$' || c == '#' || c == ':' || c == '~' || c == '?'
	|| c == '/')
		return (1);
	return (0);
}

char			*parse_tilda(char *str, t_dlist **tok)
{
	make_token(tok, NULL, TK_DEREF);
	make_token(tok, ft_strdup("HOME"), TK_NAME);
	return (++str);
}

char			*parse_nametk(char *str, t_dlist **tok, short tk)
{
	size_t	i;
	char	*new;

	i = 0;
	if (str[i] >= 48 && str[i] <= 57)
		while (str[i] && str[i] >= 48 && str[i] <= 57)
			i++;
	else
	{
		if (special_reg(str[i], tk))
		{
			make_token(tok, pull_token(str, 1), TK_NAME);
			return (str + 1);
		}
		while (str[i] && !is_separator(str[i]) && !special_reg(str[i], tk))
			i = (str[i] == '\\' && str[i + 1]) ? i + 2 : ++i;
		if (tk == CURLY && is_separator(str[i]))
			return (NULL);
	}
	new = pull_token(str, i);
	make_token(tok, markup_station(new, TK_NAME), TK_NAME);
	free(new);
	return (str + i);
}

static char		*parse_curly_br(char *str, t_dlist **tok)
{
	if (!(str = parse_nametk(++str, tok, CURLY)))
		return (NULL);
	return (++str);
}

char			*parse_deref(char *str, t_dlist **tok, t_stx **tree, short i)
{
	if (*str == '~')
		return (parse_tilda(str, tok));
	if (*str++ == '$' && *str && (*str != '\\' || *(str + 1) == '\n'))
	{
		if ((is_separator(*str) && *str != '"') || !(*str))
		{
			make_token(tok, pull_token(--str, 1), TK_EXPR);
			return (str + 1);
		}
		make_token(tok, NULL, TK_DEREF);
		if (check_branch(str, tree[DQUOTS]))
			return (parse_dquotes(str, tok, tree, 0));
		else if (check_branch(str, tree[APOFS]))
			return (parse_apofs(str, tok, tree, 0));
		else if (layer_parse_one("{z}", str))
			return (parse_curly_br(str, tok));
		else if (check_branch(str, tree[MATHS]))
			return (parse_math(str, tok, tree, 0));
		else if (check_branch(str, tree[SUBSHS]))
			return (parse_subsh(str, tok, tree, 0));
		return (parse_nametk(str, tok, i));
	}
	return (parse_comm(str, tok, tree, 0));
}
