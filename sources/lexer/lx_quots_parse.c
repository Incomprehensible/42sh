/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_quots_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/27 17:59:08 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char*   parse_apofs(char *str, t_dlist **tok, t_stx **tree, short i)
{
    size_t j;
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
        //make_token(tok, cutting_mirr_station(pull_token(str - j, j), APOFS), TK_EXPR);
    }
    return (parse_sep(++str, tok, i));
}

size_t   can_pull_tk(size_t j, char *str, t_dlist **tok, short t)
{
	t_tk_type	type;
	char		*new;

	if (t == '"')
		type = DQUOTS;
	else if (t == '\'')
		type = APOF;
	else
		type = TK_EXPR;
    if (j)
    {
        if (t == IN)
		{
			new = markup_station(pull_token(str - j, j), TK_VAR);
			make_token(tok, new, TK_VAR);
		}
        else
		{
			new = markup_station(pull_token(str - j, j), type);
			make_token(tok, new, TK_EXPR);
		}
    }
    return (0);
}

short   special_case(char br, char *str)
{
    if (br == '\n')
    {
        if (*str != ';' && *str != '\n' && *str)
            return (0);
    }
    else if (br == IN)
    {
        if (!(is_token_here(str, " in")) && !(is_token_here(str, "\tin")) && !is_sep_no_space(*str))
            return (0);
    }
    else if (!br)
    {
        if (!is_sep_no_space(*str))
            return (0);
    }
	else if (br == '"' || br == '\'')
	{
		if (*str != br)
			return (0);
	}
    else
	{
		if (*str != br && *str != ';' && *str != '\n')
            return (0);
	}
    return (1);
}

char    *parse_str_block(char *str, t_dlist **tok, t_stx **tree, short br)
{
    size_t j;
    short i;

    j = 0;
    i = 0;
    while (*str && !(!i && special_case(br, str)))
    {
        if (*str == '\\' && (++j) && (++str))
            i = i ? 0 : 1;
        else if (!i && *str == '$')
        {
            j = can_pull_tk(j, str, tok, br);
            if (!(str = parse_deref(str, tok, tree, DQUOTS)))
                return (NULL);
        }
        else
        {
            str++;
            j++;
            i = 0;
        }
    }
    can_pull_tk(j, str, tok, br);
    str = (br == '"') ? ++str : str;
    str = (*str == ' ' || *str == '\t') ? parse_empty(str, 0x0, tok) : str;
    return (parse_sep(str, tok, 0));
}

char*   parse_dquotes(char *str, t_dlist **tok, t_stx **tree, short i)
{
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