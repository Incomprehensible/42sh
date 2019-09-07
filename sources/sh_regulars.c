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

int special_meta(char meta)
{
    if (meta == '@' || meta == '^' || meta == '~' || meta == '_' || meta == '!' || meta == '%' || meta == '.' || meta == '?')
        return (1);
    return (0);
}

char *ft_process_s(char *str, char *meta)
{
    if (!(*str >= 65 && *str <= 90) && !(*str >= 97 && *str <= 122))
        return (0);
    while (*str && *str != *(meta + 1))
        str++;
    if (!(*str))
        return (0);
    return (str);
}

char *ft_process_space(char *str, char *meta)
{
    if (*str != ' ' && *str != '\t' && *str != ';' && *str != '\n')
        return (0);
    if (*meta == '_')
    {
        if (*str == ' ' || *str == '\t' || *str == ';' || *str == '\n')
            return (str);
        return (0);
    }
    while (*str && str != meta && *str != ';' && (*str == ' ' || *str == '\t' || *str == '\n'))
        str++;
    if (*str)
        return (str);
    return (0);
}

char *ft_process_seq(char *str, char *meta)
{
    char current;

    ++meta;
    while (*meta && *meta != '@')
    {
        current = *meta;
        if (*str == current)
            return (str);
        meta++;
    }
    return (0);
}

char *ft_process_any(char *str, char *meta, char *end)
{
    ++meta;
    while (*str && str != end && *str != *meta)
        str++;
    if (*str == *meta)
        return (str);
    return (0);
}

short    is_token_here(char *start, char *meta)
{
    if (*start && *start == *meta)
    {
        while (*start && *meta && *start == *meta)
        {
            start++;
            meta++;
        }
        //if (!(*meta) && (*(start - 1) == ' ' || (!(*start) || *(start) == ' ' || *start == '\t' || *start == '\n' || *start == ';')))
        if (!(*meta) && (*(start - 1) == ' ' || (!(*start) || is_separator(*start))))
            return (1);
    }
    return (0);
}

char    *ft_process_vars(int type, char *str, char *meta, t_dlist **tok)
{
    size_t i;

    i = 0;
    ++meta;
    while (*str && *str != *meta && *str != *(meta + 2))
    {
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), type);
//    if (!(*str))
//        return (0);
    return (str);
}

char    *ft_process_wall(char *str, char *meta, char *end)
{
    ++meta;
//    if (*meta == '/')
//        return (ft_process_vars(str, meta));
    while (*str && str != end && !(is_token_here(str, meta)))
        str++;
    if (!(*str))
        return (0);
    return (str);
}

char *ft_process_limits(char *str, char *end)
{
    char limit;

    if (*str && (*str == '\'' || *str == '"' || *str == '`'))
        limit = *str++;
    else
        limit = ' ';
    //we cut shit that sits between these metas ; need for math
    while (*str && str != end && *str != limit)
        str++;
    return (str - 1);
}

char *ft_process_ignore(char *str, char *meta, char *end)
{
    meta++;
    while (*str && str != end && !(special_meta(*meta)))
    {
        if (*str != *meta)
            return (0);
        str++;
        meta++;
    }
    //we ignore symbols that proceed this meta, before we approach another meta (need this for math)
    return (str);
}

char *process_reg(char *str, char *meta, char *end)
{
    if (*meta == '?')
        str = ft_process_s(str, meta);
    else if (*meta == '^' || *meta == '_')
        str = ft_process_space(str, meta);
    else if (*meta == '~')
        str = ft_process_any(str, meta, end);
    else if (*meta == '!')
        str = ft_process_wall(str, meta, end);
    else if (*meta == '.')
        str = ft_process_ignore(str, meta, end);
    else if (*meta == '%')
        str = ft_process_limits(str, end);
    else
        str = ft_process_seq(str, meta);
    return (str);
}

char *get_point(char *meta)
{
    if (*meta == '!')
    {
        while (*(meta + 1))
            meta++;
        return (meta);
    }
    else if (*meta == '.')
    {
        ++meta;
        while (*meta && !(special_meta(*(meta + 1))))
            meta++;
        return (meta);
    }
    ++meta;
    while (*meta && *meta != '@')
        meta++;
    return (meta);
}

char   *cut_brackets(char *patt, char *str, t_dlist **tok, char br)
{
    char *end;
    size_t i;
    short   j;

    i = 0;
    j = 0;
//    while (*str && *str == '(')
    while (*str && *str == br)
    {
        str++;
        j++;
    }
//    if (!(start = ft_process_wall(str, patt, str + ft_strlen(str) + 1)))
//        return (0);
    if (!(end = ft_process_wall(str, patt + j, "\0")))
        return (0);
    while (str != end && *str)
    {
        str++;
        i++;
    }
    make_token(tok, pull_token(str - i, i), math_tk);
    return (end + 2);
}

char    *reg_process(char *patt, int type, char *str, t_stx **tr, t_dlist **tok)
{
    size_t i;
    char *start;
    char *new;

    if (!(*str))
        return (str);
    str = parse_empty(str, patt, tok);
//    while ((*patt != '^' && *patt != '_') && *str == ' ')
//        str++;
    if (*str == '$')
    {
        make_token(tok, ft_strdup("$"), deref_tk);
        str++;
    }
    if (*str && type == math_tk)
    {
        if (ft_strstr(patt, "(("))
            return (cut_brackets(patt, str, tok, '('));
        else if ((str = ft_process_ignore(str, patt, "\0")))
        {
            while (*str && (*str == ' ' || *str == '\t'))
                str++;
            i = layer_parse_two(patt + 4, str);
            if (*str && (*str == '\'' || *str == '"' || *str == '`'))
                str++;
            new = pull_token(str, i);
            make_token(tok, new, type);
            str += 1;
//            str += i;
        }
    }
    else if ((i = layer_parse_two(patt, str)))
    {
        str = parse_empty(str, patt, tok);
        start = str;
//        while (*start == ' ')
//        {
//            str++;
//            start++;
//        }
        new = pull_token(start, --i);
        make_token(tok, new, type);
//        str += i;
    }
    str += i;
    if (str && *str == ';')
        str = parse_sep(str, tok, tr, 0);
    return (str);
}
