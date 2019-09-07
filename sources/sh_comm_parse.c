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

char*   parse_quotes(char *str, t_dlist **tok, t_stx **tree, short i)
{
    size_t j;

    j = 0;
    str = parse_empty(str, "", tok);
    if (*str && *str == '\'')
    {
        while (*str && *str != '\'')
        {
            str++;
            j++;
        }
        make_token(tok, pull_token(str - j, j - 1), expr_tk);
        return (parse_sep(++str, tok, tree, 0));
    }
    //do we really need to have value here? or NULL is better?
    make_token(tok, ft_strdup("\""), dquote_tk);
    str++;
    while (*str && *str != '"')
    {
        if (*str == '\\' && (j++) && (++str))
            i = 1;
        else if (!i && *str == '$')
        {
            if (j)
                make_token(tok, pull_token(str - j, j - 1), expr_tk);
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
    make_token(tok, ft_strdup("\""), dquote_tk);
    return (parse_sep(++str, tok, tree, 0));
}

//static char *parse_exec(char *str, t_dlist **tok)
//{
//    make_token(tok, ft_strdup("exec"), exec_tk);
//    return (parse_empty(str + 5, "", tok));
//}

char*   parse_comm(char *str, t_dlist **tok, t_stx **tree, short i)
{
    size_t j;

    j = 0;
//    if (*str && *str != '\\' && is_token_here(str, "exec"))
//        str = parse_exec(str, tok);
    while (*str && *str != ';')
    {
        if (*str == '\\' && (j++) && (++str))
            i = 1;
        else if (!i && (*str == '$' || *str == '(' || *str == '"' || *str == '\''))
        {
            if (j)
                make_token(tok, pull_token(str - j, j - 1), expr_tk);
            str = check_subbranch(str, tok, tree, 9);
            j = 0;
        }
        else if (!i && (*str == ' ' || *str == '\t' || *str == '\n'))
        {
            if (j)
                make_token(tok, pull_token(str - j, j - 1), expr_tk);
            str = parse_empty(str, "", tok);
            j = 0;
        }
//        else if (!i && is_separator(*str))
//        {
//            if (j)
//                make_token(tok, pull_token(str - j - 1, j - 1), expr_tk);
//            str = (*str == ' ' || *str == '\t') ? parse_empty(str, "", tok) : parse_sep(str, tok, tree, 0);
//            j = 0;
//        }
        else
        {
            str++;
            j++;
            i = 0;
        }
    }
    //str = parse_empty(str, "", tok);
    return (parse_sep(str + i, tok, tree, 0));
}

//empty_tk
//subshell
//quotes
//deref

//мы постоянно чекаем если перед пробелом стоит экранирование - тогда мы просто продолжаем парсить всю строку или же
//считать в ней символы от начала до конца
//если увидели разделитель / дереференс / другой токен и он не экранирован, продолжаем парсить
//блоки для проверки - дереференс, кавычки, разделители (все лежит в нашей парсинговой функции)
//one case is exec - one command
//another case is just comms separated by spaces and tabs
//third case is mirroring as we parse the whole command