/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_quots_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/09 12:48:12 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

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

size_t	can_pull_tk(size_t j, char *str, t_dlist **tok, int t)
{
	t_tk_type	type;

	type = (t == '"') ? DQUOTS : TK_EXPR;
	type = (t == '\'') ? APOF : type;
	if (j)
	{
		if ((short)t == IN)
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
	else if ((short)br == IN)
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
	int		flag;
	t_dlist	*last_token;

	i = 0;
	str = parse_empty(str, 0x0, tok);
	if (*str != '"')
		str = parse_comm(str, tok, tree, '"');
	if (!str)
		return (NULL);
	str = parse_empty(str, 0x0, tok);
	flag = expr_was_last(tok[1]) ? 1 : 0;
	last_token = tok[1];
	str++;
	str = parse_str_block(str, tok, tree, '"');
	if (flag)
		merge_into_expr(last_token, tok);
	return (str);
}
