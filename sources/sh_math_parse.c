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

char*   parse_math(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char *patt1;
    char *patt2;
    char *tmp;

    patt1 = "((!))";
    patt2 = ".let%";
    while (*str)
    {
        if ((tmp = reg_process(patt1, TK_MATH, str, tree, tok)) != str && tmp)
            return (tmp);
        else if ((tmp = reg_process(patt2, TK_MATH, str, tree, tok)) != str && tmp)
            return (tmp);
        str++;
        ind++;
    }
    return (str - ind);
}

