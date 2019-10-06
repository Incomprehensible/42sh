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

static char *func_args(char *str, t_dlist **tok, t_stx **tr)
{
    str++;
    while (*str && *str != '}')
    {
        if (check_branch(str, tr[FLOWS]))
            str = block_pass(FLOWS, str, tok, tr);
        else
            str = parse_comm(str, tok, tr, '}');
        str = (str) ? skip_spaces(str) : str;
        if (!sep_detected(tok[1], ';') || !check_valid_sep(tok[1]))
            return (NULL);
        str = skip_spaces(str);
    }
    if (!(*str))
        return (0);
    make_token(tok, NULL, TK_FEND);
    return (str + 1);
}

static char *get_function(char *str, char *reg1, t_dlist **tok, t_stx **tr)
{
    size_t i;
    char *reg2;

    reg2 = "{z}";
    str = parse_empty(str, 0x0, tok);
    if ((*str >= 65 && *str <= 90) || (*str >= 97 && *str <= 122))
    {
        if ((i = layer_parse_two(reg1, str)))
        {
            i = (*(str + i - 1) == ')') ? i - 2 : i;
            make_token(tok, pull_token(str, i), TK_NAME);
            str += ( *(str + i) == '(') ? i + 2 : i;
            str = parse_empty(str, reg1, tok);
            if ((layer_parse_two(reg2, str)))
                return (func_args(str, tok, tr));
            else
            {
                str = ft_process_vars(TK_EXPR, str, "/;/\n/&/|", tok);
                make_token(tok, NULL, TK_FEND);
                return (parse_sep(str, tok, 0));
            }
        }
    }
    return (NULL);
}

char*   parse_func(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char *patt0;
    char *patt2;
    char *tmp;

    patt0 = "function_";
    patt2 = "?()_";
    if ((tmp = reg_process(patt0, TK_FUNCTION, str, tok, tree)) != str && tmp)
        return (get_function(str + 8, "? ", tok, tree));
    else if ((layer_parse_two(patt2, str)))
    {
        make_token(tok, NULL, TK_FUNCTION);
        return (get_function(str, "!()", tok, tree));
    }
    return (NULL);
    //return (str - ind);
}

char*   parse_lambda(char *str, t_dlist **tok, t_stx **tree, short i)
{
    char *reg2;

    reg2 = "{z}";
    if (!(layer_parse_two(reg2, str)))
        return (NULL);
    make_token(tok, NULL, TK_LAMBDA);
    if (!(str = func_args(str, tok, tree)))
        return (NULL);
    return (parse_sep(str, tok, i));
}