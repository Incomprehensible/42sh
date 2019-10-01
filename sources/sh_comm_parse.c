/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/08/19 00:53:23 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

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
        if (*str && *str == '\\')
        {
            i++;
            str++;
        }
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), TK_EXPR);
    return (parse_sep(str + i, tok, 0));
}

static short    is_and_or(char *str)
{
    if ((*str && *str == '&' && *(str + 1) == '&') || (*str && *str == '|' && *(str + 1) == '|'))
        return (1);
    return (0);
}

short   time_for_portal(char *str)
{
    if ((*str == '$' && *(str + 1) != '=') || *str == '(' || *str == '"' || *str == '\'' ||
    *str == ';' || *str == '\n' ||  ft_isspace(*str) || is_and_or(str))
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
    while (!(special_case(stop, str)))
    {
        if (*str == '\\' && (++j) && (++str))
            i = 1;
        if (!i && *str && time_for_portal(str) && *(str + 1) != '\\')
        {
            j = can_pull_tk(j, str, tok, stop);
            if (!(str = check_subbranch(str, tok, tree, EXPRS)))
                return (NULL);
            if (sep_detected(tok[1], stop))
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

//in case we may mistakenly check subbranch - it has a mistake as it reverses arguments
//char*   parse_comm(char *str, t_dlist **tok, t_stx **tree, short stop)
//{
//    size_t j;
//    short i;
//    char *tmp;
//
//    j = 0;
//    i = 0;
//    if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
//        return (parse_exec(str, tok));
////    while (!(special_case(stop, str)) && !(is_sep_no_space(*str)) && !is_and_or(str))
//    while (!(special_case(stop, str)))
//    {
//        if (*str == '\\' && (j++) && (++str))
//            i = 1;
//        if (!i && *str && time_for_portal(str) && *(str + 1) != '\\')
//        {
////            j = can_pull_tk(j, str, tok, stop);
//            if (!(tmp = check_subbranch(str, tok, tree, EXPRS)))
//                return (NULL);
//            if (tmp != str)
//                j = can_pull_tk(j, str, tok, stop);
//            str = (tmp == str && (++j)) ? ++str : tmp;
//            if (sep_detected(tok[1], 0))
//                return (str);
//        }
//        else
//        {
//            str++;
//            j++;
//        }
//        i = (*str == '\\') ? 1 : 0;
//    }
//    can_pull_tk(j, str, tok, stop);
//    return (parse_sep(str + i, tok, 0));
//}

////rewrite - close function when we need to check subbranch and go to the portal. just return the value
//char*   parse_comm(char *str, t_dlist **tok, t_stx **tree, short i)
//{
//    size_t j;
//
//    j = 0;
//    if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
//        return (parse_exec(str, tok));
//    while (*str && !(is_sep_no_space(*str)))
//    {
//        if (*str == '\\' && (j++) && (++str))
//            i = 1;
//        if (!i && *str && time_for_portal(*str) && *(str + 1) != '\\')
//        {
//            if (j)
//                make_token(tok, pull_token(str - j, j - 1), TK_EXPR);
//            if (!(str = check_subbranch(str, tok, tree, EXPRS)))
//                return (NULL);
//            j = 0;
//        }
//        else if (!i && (*str == ' ' || *str == '\t' || *str == '\n'))
//        {
//            if (j)
//                make_token(tok, pull_token(str - j, j), TK_EXPR);
//            str = parse_empty(str, 0x0, tok);
//            j = 0;
//        }
//        else
//        {
//            if (!i)
//            {
//                if (*str != ' ' && is_separator(*str))
//                {
//                    if (j)
//                        make_token(tok, pull_token(str - j, j), TK_EXPR);
//                    str = parse_sep(str, tok, 0);
//                    str = parse_empty(str, 0x0, tok);
//                    j = 0;
//                }
//            }
//            i = (*str == '\\') ? 1 : 0;
//            str++;
//            j++;
//        }
//    }
//    if (j)
//        make_token(tok, pull_token(str - j, j), TK_EXPR);
//    return (parse_sep(str + i, tok, 0));
//}