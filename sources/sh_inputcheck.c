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

short   count_points(char *str, char *point)
{
    char *start;
    short i;
    char    *patt;

    start = str;
    i = 0;
    patt = "if";
    while (start)
    {
        start = ft_strstr(start, point);
        if (start)
        {
            while (*start && *start == *patt++)
                start++;
            i++;
            patt -= 3;
        }
    }
    return (i);
}

short   check_ifs(char *str)
{
    short i;
    short j;
    char *point;

    i = count_points(str, "if");
    j = 0;
    point = str;
    while (point && j != i)
    {
        point = ft_strstr(point, "fi");
        if (point)
            j++;
    }
    return (j == i ? 1 : 0);
}

short   check_while(char *str)
{
    short i;
    short j;
    char *point;

    i = count_points(str, "while");
    j = 0;
    point = str;
    while (point && j != i)
    {
        point = ft_strstr(point, "done");
        if (point)
            j++;
    }
    return (j == i ? 1 : 0);
}

short   check_until(char *str, short add)
{
    short i;
    short j;
    char *point;

    i = count_points(str, "until");
    j = 0;
    point = str;
    while (point && j != i)
    {
        point = ft_strstr(point, "done");
        if (point)
            j++;
    }
    j -= add;
    return (j == i ? 1 : 0);
}

short   check_for(char *str, short add)
{
    short i;
    short j;
    char *point;

    i = count_points(str, "for");
    j = 0;
    point = str;
    while (point && j != i)
    {
        point = ft_strstr(point, "done");
        if (point)
            j++;
    }
    j -= add;
    return (j == i ? 1 : 0);
}

short   script_input(char *str)
{
    short flag;
    short i;

    flag = 0;
    i = 0;
    if (is_token_here(str, "if"))
        flag = check_ifs(str);
    if (is_token_here(str, "while"))
    {
        i = check_while(str);
        flag = i;
    }
    if (is_token_here(str, "until"))
    {
        i += check_until(str, i);
        flag = i;
    }
    if (is_token_here(str, "for"))
    {
        i += check_for(str, i);
        flag = i;
    }
    return (flag);
}

short   count_objects(char *str, char br, char rb)
{
    short flag;
    short mir;

    flag = 0;
    while (*str)
    {
        mir = 0;
        if (*str++ == '\\')
            mir = 1;
        if (*str == br && !mir)
            flag++;
        else if (*str == rb && !mir)
            flag--;
        str++;
    }
    return (flag ? 0 : 1);
}

short   brackets_closed(char *str)
{
    if (ft_strchr(str, '('))
        if (!count_objects(str, '(', ')'))
            return (0);
    if (ft_strchr(str, '{'))
    {
        if (!count_objects(str, '{', '}'))
            return (0);
    }
    return (1);
}

short   quotes_closed(char *str)
{
    if (ft_strchr(str, '\''))
        if (!count_objects(str, '\'', '\''))
            return (0);
    if (ft_strchr(str, '"'))
    {
        if (!count_objects(str, '"', '"'))
            return (0);
    }
    if (ft_strchr(str, '`'))
    {
        if (!count_objects(str, '`', '`'))
            return (0);
    }
    return (1);
}

short   pipes_closed(char *str)
{
    if (*(str + ft_strlen(str) - 1) == '|' && *(str + ft_strlen(str) - 2) != '\\')
        return (0);
    return (1);
}

//check for closed pipes as well!! input can't finish with a pipe opened
//we check if functions' input is finished - whether our brackets of commands within func definition are closed or if there is
//only function or fname written
//instead of find_token we must only search for meta words across the whole string - it differs from check-branch as we
//don't look for separators and analyze string not in parts but the whole.
//as first meta is found and we check every meta with layer_parse_one, we go script_input
//we search for quotes and brackets in the same fashion - first search for the first bracket, if it's found go check brackets etc.
short   input_finished(char *str, t_stx **tree)
{
    short i;
    short id;
    static short kek;

    //if we have unclosed heredoc, input not finished
    //we may check static variable in outer func
    //if (!check_heredoc()) where we store a flag
    if (kek)
    {
        if (!brackets_closed(str))
            return (-1);
        if (!quotes_closed(str))
            return (-1);
        if (!pipes_closed(str))
            return (-1);
        kek = 0;
    }
    i = find_token(tree, str);
    id = 1;
    if (i == 1)
        id = script_input(str);
    return (id ? i : -1);
}