/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_proc_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/07 08:35:18 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char *valid_sign(char *str)
{
    char c;
    short i;

    c = *str;
    i = 0;
    while (*str && !(ft_isspace(*str)) && *str != '(')
    {
        if (*str != c)
            return (NULL);
        str++;
        i++;
    }
    return (i > 1 ? NULL : str);
}

short    is_redir(char *str)
{
    str = skip_spaces(str);
    if (*str && (*str == '<' || *str == '>'))
        return (1);
    return (0);
}

static short   validate_proc(char *str)
{
    if (*str != '<' && *str != '>' && !(ft_isspace(*str)))
        return (-1);
    if (!(str = valid_sign(str)))
        return (0);
    if (*str != '(')
    {
        if (is_redir(str))
            return (-1);
        return (0);
    }
    return (1);
}

char*   parse_proc(char *str, t_dlist **tok, t_stx **tree, short i)
{
    char    *tmp;
	short	choice;
    t_tk_type  type;

    if (*str == '\\')
        return (parse_comm(str, tok, tree, i));
	choice = i;
	// str = (*str != '<' && *str != '>') ? parse_comm(str, tok, tree, '<') : str;
    if (!(i = (validate_proc(str))))
        return (NULL);
    if (i < 0)
        return (parse_redir(str, tok, tree, 0));
    if (*str++ == '<')
    {
        type = (choice == PROF) ? TK_PROF_OUT : TK_PROC_OUT;
        if ((tmp = reg_process("(z)", type, str, tok, tree)) == str)
            return (NULL);
    }
    else
    {
        type = (choice == PROF) ? TK_PROF_IN : TK_PROC_IN;
        if ((tmp = reg_process("(z)", type, str, tok, tree)) == str)
            return (NULL);
    }
    str = tmp;
    return (str);
}
