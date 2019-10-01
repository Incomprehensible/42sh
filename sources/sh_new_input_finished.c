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

size_t   mirror_passes(char *str)
{
    size_t i;

    i = 0;
    if (ft_isspace(str[i]))
        i++;
    i++;
    return (i);
}

size_t  skip_field(char *str, char skip)
{
    size_t jump;
    size_t add;

    jump = 0;
    add = 0;
    ++str;
    while (*str && *str != skip)
    {
        jump++;
        if (*str == '\\')
        {
            add = mirror_passes(str);
            str += add;
        }
        else
            str++;
    }
    if (!(*str))
        return (0);
    return (++jump + add);
}

short   q_closed(char *str, char q, char q1, char q2)
{
    size_t jump;
    short times;

    times = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else
        {
            if (*str == q)
                times = (times) ? --times : ++times;
            if (!times && (*str == q1 || *str == q2))
            {
                if (!(jump = skip_field(str, *str)))
                    return (0);
                str += jump;
            }
            str++;
        }
    }
    return (times ? 0 : 1);
}

short   quotes_closed(char *str)
{
    if (!(q_closed(str, '\'', '"', '`')))
        return (0);
    if (!(q_closed(str, '"', '\'', '`')))
        return (0);
    if (!(q_closed(str, '`', '\'', '"')))
        return (0);
    return (1);
}

short   is_it_q(char c)
{
    if (c == '\'' || c == '"' || c == '`')
        return (1);
    return (0);
}

short   br_closed(char *str, char strt, char fin)
{
    short times;

    times = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else
        {
            if (*str == strt)
                times++;
            if (*str == fin)
                times--;
            if (is_it_q(*str))
                str += skip_field(str, *str);
            str++;
        }
    }
    return (times ? 0 : 1);
}

//here we enter and check if any following symbol is a quote - then check type of quote - then go pass through this and  skip some bytes
short   brackets_closed(char *str)
{
    if (!(br_closed(str, '{', '}')))
        return (0);
    if (!(br_closed(str, '[', ']')))
        return (0);
    if (!(br_closed(str, '(', ')')))
        return (0);
    return (1);
}

static short    is_and_closed(char *str, size_t size)
{
    if (*(str + size - 1) == '&' && (size - 2) && *(str + size - 2) == '&')
    {
        if ((size - 3) && *(str + size - 2) == '\\')
            return (1);
        return (0);
    }
    return (1);
}

short   input_closed(char *str)
{
    size_t size;

    size = ft_strlen(str);
    if (*(str + size - 1) == '|' && (size - 2) && *(str + size - 2) != '\\')
        return (0);
    if (*(str + size - 1) == '\\' && (size - 2) && *(str + size - 2) != '\\')
        return (0);
    if (!is_and_closed(str, size))
        return (0);
    return (1);
}

int  validate_ifs(char *str)
{
    int num;

    num = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str))
        {
            str += skip_field(str, *str);
            ++str;
        }
        else
        {
            if (is_token_here(str, "if"))
                num++;
            else if (is_token_here(str, "fi"))
                num--;
            str++;
        }
    }
    return (num);
}

short   are_tokens_here(char *str)
{
    if (is_token_here(str, "while"))
        return (1);
    if (is_token_here(str, "for"))
        return (1);
    if (is_token_here(str, "until"))
        return (1);
    return (0);
}

int  validate_cycles(char *str, char *meta)
{
    int num;

    num = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str))
        {
            str += skip_field(str, *str);
            ++str;
        }
        else
        {
            if (are_tokens_here(str))
                num++;
            else if (is_token_here(str, meta))
                num--;
            str++;
        }
    }
    return (num);
}

short   scripts_closed(char *str)
{
    if (validate_ifs(str) > 0)
        return (1);
    if (validate_cycles(str, "done") > 0)
        return (1);
    return (0);
}

short   input_finished(char *str)
{
    if (!brackets_closed(str))
        return (-1);
    if (!quotes_closed(str))
        return (-1);
    if (!input_closed(str))
        return (-1);
    if (scripts_closed(str))
        return (-1);
    return (1);
}

