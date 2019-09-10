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

short   is_separator(char str)
{
    if (str != ' ' && str != '\t' && str != '\n' && str != ';' && str != '&' && str != '|')
        return (0);
    return (1);
}

char    *get_deref(char *str, t_stx **tr, t_dlist **tok)
{
    size_t i;

    i = 0;
    make_token(tok, ft_strdup("$"), TK_DEREF);
    str++;
    if (*str && *str == '(' && check_branch(str, tr[8]))
        return (block_pass(TK_SUBSHS, str, tok, tr));
    while (*str && !(is_separator(*str)))
    {
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i - 1), TK_NAME);
    return (parse_sep(str, tok, tr, 0));
}

static void    get_envar(char *str, t_stx **tr, t_dlist **tok, short i)
{
    while (*str && *str != '=' && *str != '+' && *str != '-')
    {
//        if (*str == '\\')
//            str++;
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), TK_NAME);
    i = (*str == '+' || *str == '-') ? 2 : 1;
    make_token(tok, pull_token(str, i), TK_ASSIGM);
    str += i;
    i = 0;
    if (*str == '\\' && (*(str + 1) == ' ' || *(str + 1) == '\t'))
        str += 2;
    else if (*str == '$' && *(str - 1) != '\\' && !is_separator(*(str + 1)))
    {
        get_deref(str, tr, tok);
        return ;
    }
    while (*str && !(is_separator(*str)))
    {
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), TK_VALUE);
}

//if we have dollar sign in front of name_tk, we process it as whole command
char*   parse_envar(char *str, t_dlist **tok, t_stx **tree, short i)
{
    char *patt1;
    char *patt2;

    patt1 = "~=~_";
    patt2 = "~\\ =\\ ~_";
    if ((i = layer_parse_two(patt1, str)))
        get_envar(str, tree, tok, 0x0);
    else if ((i = layer_parse_two(patt2, str)))
        get_envar(str, tree, tok, 0x1);
    else
        str = parse_comm(str, tok, tree, 0);
    return (parse_sep(str + i, tok, tree, 0));
}