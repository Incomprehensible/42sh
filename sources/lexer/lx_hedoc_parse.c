/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_hedoc_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/22 23:46:53 by bomanyte         ###   ########.fr       */
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
        if (*str == '\\' && ++i)
            flag = 0;
        else
        {
            i++;
            flag = 1;
        }
        str = (*str && *(str + 1) == '\n' && ++i) ? str + 2 : ++str;
    }
    if (!i)
        return (NULL);
    make_token(tok, cutting_mirr_station(pull_token(str - i, i), TK_HERED), TK_WORD);
    return (str);
}

char*   parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i)
{
    str = parse_comm(str, tok, tree, '<');
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