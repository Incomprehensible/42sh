/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_inputcheck.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/09/15 23:49:09 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char    *pull_word(char *str, t_dlist **tok, short i)
{
    short flag;

    flag = 1;
    str = (*str == '\\' && !(flag = 0)) ? ++str : str;
    i = 0;
    while (*str && !(flag && is_separator(*str)))
    {
        if (*str == '\\')
            flag = 0;
        else
        {
            i++;
            flag = 1;
        }
        str++;
    }
    if (!i)
        return (NULL);
    make_token(tok, pull_token(str - i, i), TK_WORD);
    return (str);
}

static char *pull_command(char *str, t_dlist **tok)
{
    size_t comm;
    short flag;

    comm = 0;
    flag = 1;
    while (*str && !(flag && ft_isspace(*str)) && !(flag && *str == '<'))
    {
        flag = (*str == '\\' && flag) ? 0 : 1;
        str++;
        comm++;
    }
    if (comm)
        make_token(tok, pull_token(str - comm, comm), TK_EXPR);
    return (skip_spaces(str));
}

char*   parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i)
{
    str = pull_command(str, tok);
    while (*str++ == '<')
        i++;
    if (i > 3 || i < 2)
        return (NULL);
    if (layer_parse_one("(z)", --str))
        return (NULL);
    make_token(tok, pull_token(str - i, i), TK_HERED);
    str = skip_spaces(str);
    if (!(*str) || is_sep_no_space(*str))
        return (NULL);
    return (pull_word(str, tok, 0));
}

//static char     *pull_word(char *str)
//{
//    size_t  i;
//    short   size;
//
//    i = ft_strlen(str) - 1;
//    size = 0;
//    while (i && str[i] != '<')
//        i--;
//    while (i && size <= 1)
//    {
//        while (i && str[i] == '<')
//        {
//            i--;
//            size++;
//        }
//        if (size > 1)
//            break;
//        else
//            size = 0;
//    }
//    if (!i)
//        return (NULL);
//    str += i + size + 1;
//    i = 0;
//    while (*str && !(is_separator(str[i])))
//    {
//        str++;
//        i++;
//    }
//    return (pull_token(str - i, i));
//}

//static short    get_hedoc(char *str, int id)
//{
//    static char *word;
//
//    if (!id)
//    {
//        word = pull_word(str);
//        if (!word)
//            return (-1);
//    }
//    else
//    {
//        str += ft_strlen(str) - 1;
//        while (*str != '<')
//        {
//            if (!(ft_strcmp(word, str)) && *(str - 1) != '<')
//            {
//                free(word);
//                return (1);
//            }
//            str--;
//        }
//    }
//    return (0);
//}