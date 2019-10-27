/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_comm_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/27 15:48:52 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*skip_comment(char *str)
{
	while (*str && *str != '\n')
		str++;
	return (str);
}

char    *parse_exec(char *str, t_dlist **tok)
{
    size_t i;

    i = 0;
    make_token(tok, pull_token(str, 4), TK_EXEC);
    str += 4;
   if (ft_isspace(*str))
       str = parse_empty(str, 0x0, tok);
    while (*str && !is_sep_no_space((*str)))
    {
        if ((*str && *str == '\\') || ((*str == '<' || *str == '>') && *(str + 1) == '&'))
        {
            str++;
            i++;
        }
        str++;
        i++;
    }
	make_token(tok, markup_station(pull_token(str - i, i), TK_EXPR), TK_EXPR);
    // make_token(tok, pull_token(str - i, i), TK_EXPR);
    return (parse_sep(str, tok, 0));
}

static short    is_and_or(char *str)
{
    if ((*str && *str == '&' && *(str + 1) == '&') || (*str && *str == '|' && *(str + 1) == '|'))
        return (1);
    return (0);
}

static short   redir_detected(char *str, t_stx **tree, short stop)
{
	if (stop == '<')
		return (0);
    str = (*str == '\\') ? str + 2 : str;
    while (*str && !(is_sep_no_space(*str)))
    {
        if (check_branch(str, tree[REDIR]))
            return (1);
        str = (*str == '\\') ? str + 2 : ++str;
    }
    return (0);
}

static short valid_deref(char *str)
{
    if (*str == '$' && *(str + 1) != '=' && !ft_isspace(*(str + 1)))
        return (1);
    return (0);
}

short   time_for_portal(char *str, t_stx **tree, short stop)
{
    if (*str == '#' || (*str == '=' && !(ft_isspace(*(str + 1)))) || (*str == '+' && *(str + 1) == '=' && !(ft_isspace(*(str + 2)))) ||
	redir_detected(str, tree, stop) || valid_deref(str) || *str == '(' || *str == '"' || *str == '\'' ||
        is_sep_no_space(*str) || ft_isspace(*str) || is_token_here(str, "exec"))
        return (1);
    return (0);
}

char*   parse_comm(char *str, t_dlist **tok, t_stx **tree, short stop)
{
    size_t j;
    short i;

    j = 0;
    i = 0;
    if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
        return (parse_exec(str, tok));
    while (!(special_case(stop, str)) && *str)
    {
        if (*str == '\\' && (++j) && (++str))
            i = 1;
		if (!i && !j && *str == '\n')
			str = parse_empty(str, 0x0, tok);
		else if (!i && *str && time_for_portal(str, tree, stop))
        {
            j = can_pull_tk(j, str, tok, stop);
            if (!(str = check_subbranch(str, tok, tree)))
                return (NULL);
            if (sep_detected(tok[1]))
                return (str);
        }
		else
        {
            str++;
            j++;
        }
        i = (*str == '\\') ? 1 : 0;
    }
    can_pull_tk(j, str, tok, stop);
    return (parse_sep(str + i, tok, 0));
}