/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_quots_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/21 23:32:25 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*parse_apofs(char *str, t_dlist **tok, t_stx **tree, short i)
{
	size_t	j;
	char	*new;

	j = 0;
	if (!is_q(*str))
		if (!(str = parse_comm(str, tok, tree, '\'')))
			return (NULL);
	if (*str && *str == '\'')
	{
		++str;
		while (*str && *str != '\'')
		{
			if (*str == '\\')
			{
				str++;
				j++;
			}
			str++;
			j++;
		}
		new = markup_station(pull_token(str - j, j), APOF);
		make_token(tok, new, TK_EXPR);
	}
	return (parse_sep(++str, tok, i));
}

void	yes_we_can(char *s, t_dlist **tk, t_tk_type type, size_t j)
{
	char	*new;
	char	*tmp;

	tmp = pull_token(s - j, j);
	new = markup_station(tmp, type);
	type = (type == TK_VAR) ? TK_VAR : TK_EXPR;
	make_token(tk, new, type);
	if (tmp)
		free(tmp);
}

size_t	can_pull_tk(size_t j, char *str, t_dlist **tok, short t)
{
	t_tk_type	type;
	char		*new;
	char		*tmp;

	type = (t == '"') ? DQUOTS : 0;
	type = (t == '\'') ? APOF : type;
	type = type ? type : TK_EXPR;
	if (j)
	{
		if (t == IN)
			yes_we_can(str, tok, TK_VAR, j);
		else
			yes_we_can(str, tok, type, j);
	}
	return (j);
}

short	special_case(char br, char *str)
{
	if (br == '\n')
		return (*str != ';' && *str != '\n' && *str) ? 0 : 1;
	else if (br == IN)
	{
		if (!(is_token_here(str, " in")) && !(is_token_here(str, "\tin")) &&
		!is_sep_no_space(*str))
			return (0);
	}
	else if (!br)
		return (!is_sep_no_space(*str)) ? 0 : 1;
	else if (br == '"' || br == '\'')
		return (*str != br) ? 0 : 1;
	else
		return (*str != br && *str != ';' && *str != '\n') ? 0 : 1;
	return (1);
}

char	*parse_dquotes(char *str, t_dlist **tok, t_stx **tree, short i)
{
	i = 0;
	str = parse_empty(str, 0x0, tok);
	if (*str != '"')
		str = parse_comm(str, tok, tree, '"');
	if (!str)
		return (NULL);
	str = parse_empty(str, 0x0, tok);
	str++;
	str = parse_str_block(str, tok, tree, '"');
	return (str);
}
