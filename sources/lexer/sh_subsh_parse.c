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

static short    special_reg(char c, short tk)
{
    if (tk == CURLY)
    {
        if (c == '}')
            return (1);
        return (0);
    }
    if (c == '!' || c == '*' || c == '$' || c == '#' || c == '?')
        return (1);
    return (0);
}

char    *parse_nametk(char *str, t_dlist **tok, short tk)
{
    size_t i;

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
    }
    make_token(tok, pull_token(str, i), TK_NAME);
    return (str + i);
}

static char    *parse_curly_br(char *str, t_dlist **tok)
{
    str = parse_nametk(++str, tok, CURLY);
    return (++str);
}

//char    *parse_nametk(char *str, t_dlist **tok)
//{
//    size_t i;
//
//    i = 0;
//    if (str[i] >= 48 && str[i] <= 57)
//        while (str[i] && str[i] >= 48 && str[i] <= 57)
//            i++;
//    else
//    {
//        if (special_reg(str[i]))
//        {
//            make_token(tok, pull_token(str, 1), TK_NAME);
//            return (str + 1);
//        }
//        while (str[i] && !is_separator(str[i]) && str[i] != '$')
//            i = (str[i] == '\\' && str[i + 1]) ? i + 2 : ++i;
//    }
//    make_token(tok, pull_token(str, i), TK_NAME);
//    return (str + i);
//}

char    *parse_deref(char *str, t_dlist **tok, t_stx **tree, short i)
{
   // str = parse_empty(str, 0x0, tok);
    if (*str++ == '$' && *str != '\\')
    {
        if ((is_separator(*str) && *str != '"') || !(*str))
        {
            make_token(tok, pull_token(--str, 1), TK_EXPR);
            return (str + 1);
        }
        make_token(tok, NULL, TK_DEREF);
        if (i && check_branch(str, tree[DQUOTS]))
            return (parse_dquotes(str, tok, tree, i));
        else if (layer_parse_one("{z}", str))
            return (parse_curly_br(str, tok));
        else if (check_branch(str, tree[MATHS]))
            return (parse_math(str, tok, tree, i));
        else if (check_branch(str, tree[SUBSHS]))
            return (parse_subsh(str, tok, tree, i));
        return (parse_nametk(str, tok, 0));
    }
    return (parse_comm(str, tok, tree, 0));
}

char   *parse_subsh(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char *patt;
    char *tmp;

    patt = "(z)";
    if (*str == '\\' || *str != '(')
        return (parse_comm(str, tok, tree, ind));
    if ((tmp = reg_process(patt, TK_SUBSH, str, tok, tree)) == str)
        return (NULL);
    str = tmp;
    return (str);
}