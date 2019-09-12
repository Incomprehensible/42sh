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

//rewrite
char    *parse_mirror(char *str, t_dlist **tok, t_stx **tr, short i)
{
    char *new;

    if (*str && (*(str + 1) == ' ' || *(str + 1) == '\t') )
    {
        *new = *(++str);
        make_token(tok, ft_strdup(new), TK_EXPR);
        return (++str);
    }
    while (*str && *str != ' ' && *str != '\t' && *str != '\n')
    {
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), TK_EXPR);
    if (*str == ';')
        str = parse_sep(str, tok, 0);
//    if (*str == ' ' || *str == '\t')
//        str = parse_empty(str, 0x0, tok);
    return (str);
}

char   *parse_subsh(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char *patt;

    patt = "(~)";
    if (*str == '\\')
        return (parse_comm(str, tok, tree, ind));
        //return (parse_mirror(str, tok, tree, ind));
    str = reg_process(patt, TK_SUBSH, str, tree, tok);
    return (str);
}