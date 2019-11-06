/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_regulars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/06 04:57:06 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

int special_meta(char meta)
{
    if (meta == '@' || meta == '^' || meta == '~' || meta == '_' || meta == '!' || meta == '%' || meta == '.'
    || meta == '?' || meta == 'z' || meta == 'w')
        return (1);
    return (0);
}

static short is_meta(char *str, char meta)
{
    if (meta == ' ' || meta == '^')
    {
        if (meta == ' ' && (*str == ' ' || *str == '\t' || *str == ';' || *str == '\n' || !(*str)))
            return (1);
        if (meta == '^' && ft_process_space(str, &meta))
            return (1);
    }
    return (0);
}

short is_ariphmetic(char str)
{
    if (str == '<' || str == '>' || str == '=')
        return (1);
    return (0);
}

char *ft_process_s(char *str, char *meta)
{
    if ((!(*str >= 65 && *str <= 90) && !(*str >= 97 && *str <= 122)) &&
    *str != '_' && !(ft_isdigit(*str)))
        return (0);
    while (*str && !is_separator(*str) && *str != *(meta + 1))
    {
        if (is_prefix(*str) || is_ariphmetic(*str))
            return (NULL);
        str = (*str == '\\') ? str + 2 : ++str;
    }
    if (!(*str) && !(is_meta(str, *(meta + 1))))
        return (0);
    return (str);
}

char *ft_process_space(char *str, char *meta)
{
    if (*meta == '_')
    {
        if (*str == ' ' || *str == '\t' || !(*str) || is_sep_no_space(*str) || *str == '(')
            return (str);
        return (0);
    }
    if (*str != ' ' && *str != '\t' && *str)
        return (0);
    while (*str && str != meta && !is_sep_no_space(*str) && (*str == ' ' || *str == '\t'))
        str++;
    if (*str)
        return (str);
    return (0);
}

char    *ft_process_trash(char *str, char *meta)
{
    ++meta;
    while (*str && *str != *meta)
    {
        if (*str == '\\')
            str++;
        str++;
    }
    if (*str == *meta)
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

char *ft_process_all(char *str, char *meta)
{
    short flag;

    flag = 0;
    ++meta;
    while (*str && *str != *meta)
    {
		if (*meta == ')' && (*str == '"' || *str == '\''))
			str += skip_field(str, *str);
        if (*str && !is_separator(*str))
            flag = 1;
        if (*str == '\\' && *(str + 1) && (flag = 1))
            str++;
        str++;
    }
    if (*str == *meta && flag)
        return (str);
    return (0);
}

char *ft_process_any(char *str, char *meta)
{
    ++meta;
    while (*str && !is_sep_no_space(*str) && *str != *meta && !(is_meta(str, *meta)))
    {
        str = (*str == '\\') ? str + 2 : ++str;
    }
    if (*str == *meta || (is_meta(str, *meta)))
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
        if (!(*meta) && (*(start - 1) == ' ' || (!(*start) || is_separator(*start))))
            return (1);
    }
    return (0);
}

char    *ft_process_vars(t_tk_type type, char *str, char *meta, t_dlist **tok)
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
    return (str);
}

char    *ft_process_wall(char *str, char *meta)
{
    ++meta;
    while (*str && !is_sep_no_space(*str) && !is_token_here(str, meta))
        str++;
    if (is_sep_no_space(*str))
        return (NULL);
    if (!(*str))
        return (0);
    return (str);
}

char *ft_process_limits(char *str)
{
    char limit;

    if (*str && (*str == '\'' || *str == '"' || *str == '`'))
        limit = *str++;
    else
        limit = ' ';
    while (*str && !is_sep_no_space(*str) && *str != limit)
        str++;
    return (str - 1);
}

char *ft_process_ignore(char *str, char *meta)
{
    meta++;
    while (*str && !is_sep_no_space(*str) && !(special_meta(*meta)))
    {
        if (*str != *meta)
            return (0);
        str++;
        meta++;
    }
    return (str);
}

char *process_reg(char *str, char *meta)
{
    if (*meta == '?')
        str = ft_process_s(str, meta);
    else if (*meta == '^' || *meta == '_')
        str = ft_process_space(str, meta);
    else if (*meta == '~')
        str = ft_process_any(str, meta);
    else if (*meta == '!')
        str = ft_process_wall(str, meta);
    else if (*meta == '.')
        str = ft_process_ignore(str, meta);
    else if (*meta == '%')
        str = ft_process_limits(str);
    else if (*meta == 'z')
        str = ft_process_all(str, meta);
    else if (*meta == 'w')
        str = ft_process_trash(str, meta);
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

char    *reg_process(char *patt, t_tk_type type, char *str, t_dlist **tok, t_stx **tr)
{
    size_t i;
    char *new;

    if (!(*str))
        return (str);
    str = parse_empty(str, patt, tok);
    if (*str == '$' && !(is_separator(*(str + 1))))
        return (parse_deref(str, tok, tr, 0));
	else if (type == TK_SUBSH || type == TK_PROF_IN || type == TK_PROF_OUT ||
		type == TK_PROC_OUT || type == TK_PROC_IN)
		return (pull_subsh(str, tok, type));
    if ((i = layer_parse_two(patt, str)))
    {
        if (type == TK_FUNCTION)
            new = NULL;
        else
            new = pull_token(str, --i);
        make_token(tok, new, type);
    }
    str += i;
    return (str);
}