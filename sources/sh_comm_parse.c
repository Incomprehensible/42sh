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

static char *pull_comm(char *str, t_dlist **tok)
{
    short i;

    i = 0;
    while (*str && *str != '"')
    {
        if (i && *str == ' ' && *(str - 1) != '\\')
        {
            make_token(tok, pull_token(str - i, i), TK_EXPR);
            str = parse_empty(str, 0x0, tok);
            i = 0;
        }
        else
        {
            i++;
            str++;
        }
    }
    if (i)
        make_token(tok, pull_token(str - i, i), TK_EXPR);
    return (str);
}

char*   parse_quotes(char *str, t_dlist **tok, t_stx **tree, short i)
{
    size_t j;

    j = 0;
    str = parse_empty(str, 0x0, tok);
    if (*str != '"')
        str = pull_comm(str, tok);
    if (*str && *str == '\'')
    {
        while (*str && *str != '\'')
        {
            str++;
            j++;
        }
        make_token(tok, pull_token(str - j, j - 1), TK_EXPR);
        return (parse_sep(++str, tok, 0));
    }
    //do we really need to have value here? or NULL is better?
    make_token(tok, ft_strdup("\""), TK_DQUOTE);
    str++;
    while (*str && *str != '"')
    {
        if (*str == '\\' && (j++) && (++str))
            i = 1;
        else if (!i && *str == '$')
        {
            if (j)
                make_token(tok, pull_token(str - j, j - 1), TK_EXPR);
            str = get_deref(str, tree, tok);
            j = 0;
        }
        else
        {
            str++;
            j++;
            i = 0;
        }
    }
    make_token(tok, ft_strdup("\""), TK_DQUOTE);
    return (parse_sep(++str, tok, 0));
}

char*   parse_comm(char *str, t_dlist **tok, t_stx **tree, short i)
{
    size_t j;

    j = 0;
    while (*str && *str != ';')
    {
        if (*str == '\\' && (j++) && (++str))
            i = 1;
        else if (!i && (*str == '$' || *str == '(' || *str == '"' || *str == '\''))
        {
            if (j)
                make_token(tok, pull_token(str - j, j - 1), TK_EXPR);
            str = check_subbranch(str, tok, tree, TK_EXPRS);
            j = 0;
        }
        else if (!i && (*str == ' ' || *str == '\t' || *str == '\n'))
        {
            if (j)
                make_token(tok, pull_token(str - j, j), TK_EXPR);
            str = parse_empty(str, 0x0, tok);
            j = 0;
        }
        else
        {
            str++;
            j++;
            i = 0;
        }
    }
    if (j)
        make_token(tok, pull_token(str - j, j), TK_EXPR);
    return (parse_sep(str + i, tok, 0));
}

//мы постоянно чекаем если перед пробелом стоит экранирование - тогда мы просто продолжаем парсить всю строку или же
//считать в ней символы от начала до конца
//если увидели разделитель / дереференс / другой токен и он не экранирован, продолжаем парсить
//блоки для проверки - дереференс, кавычки, разделители (все лежит в нашей парсинговой функции)