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

short    unexpected_token(void)
{
    ft_putstr("bash: syntax error occured.\n");
    return (-1);
}

//short is_it_sep(char str)
//{
//    if (str != '\n' && str != ';')
//        return (0);
//    return (1);
//}

//short   count_points(char *str, char *point)
//{
//    char *start;
//    short i;
//
//    start = str;
//    i = 0;
//    while (start)
//    {
//        start = ft_strstr(start, point);
//        if (start)
//        {
//            while (*start && !is_separator(*start))
//                start++;
//            i++;
//        }
//    }
//    return (i);
//}
//
////changed to count if before done and if we have another if / done and if we have delimeter in front
//short   sep_is_here(char *str, char *point)
//{
//    short flag;
//
//    flag = 0;
//    while (point-- != str)
//    {
//        while (point != str && (*point == ' ' || *point == '\t'))
//            --point;
//        if (*point && is_it_sep(*point))
//        {
//            flag = 1;
//            break;
//        }
//		while ((*point != ' ' || *point != '\t' || is_it_sep(*point)) && point != str)
//			--point;
//		if (point != str)
//			if (is_token_here(point, "if") || is_token_here(point, "done"))
//				flag = 1;
//        return (flag);
//    }
//    return (flag);
//}
//
//short   check_ifs(char *str)
//{
//    short i;
//    short j;
//    char *point;
//
//    i = count_points(str, "if");
//    j = 0;
//    point = str;
//    while (point && j != i)
//    {
//        point = ft_strstr(point, "fi");
//        if (point && point != str && *(point - 1) != '\\')
//            if (sep_is_here(str, point))
//                j++;
//    }
//    return (j == i ? 1 : 0);
//}
//
//short   check_cycle(char *str, char *meta)
//{
//    short i;
//    short j;
//    char *point;
//
//    i = count_points(str, meta);
//    j = 0;
//    point = str;
//    while (point && j != i)
//    {
//        point = ft_strstr(point, "done");
//		if (point && point != str && *(point - 1) != '\\')
//        	if (sep_is_here(str, point))
//            	j++;
//    }
//    return (j);
//}
//
//short   script_input(char *str)
//{
//    short i;
//
//    while (*str && (*str == ' ' || *str == '\t'))
//        str++;
//    if (ft_strstr(str, "if"))
//        if (!(check_ifs(str)))
//			return (0);
//	i = count_points(str, "done");
//    if (ft_strstr(str, "while"))
//        i -= check_cycle(str, "while");
//    if (ft_strstr(str, "until"))
//        i -= check_cycle(str, "until");
//    if (ft_strstr(str, "for"))
//        i -= check_cycle(str, "for");
//    return (i ? 0 : 1);
//}
//
//// short   script_input(char *str)
//// {
////     short flag;
////     short i;
//
////     flag = 0;
////     i = 0;
////     while (*str && (*str == ' ' || *str == '\t'))
////         str++;
////     if (ft_strstr(str, "if"))
////         flag = check_if_while(str, "if", "fi");
////     if (ft_strstr(str, "while"))
////     {
////         i = check_if_while(str, "while", "done");
////         flag = i;
////     }
////     if (ft_strstr(str, "until"))
////     {
////         i += check_until_for(str, "until", i);
////         flag = i;
////     }
////     if (ft_strstr(str, "for"))
////     {
////         i += check_until_for(str, "for", i);
////         flag = i;
////     }
////     return (flag);
//// }
//
//short   count_objects(char *str, char br, char rb)
//{
//    short i;
//    short mir;
//    short flag;
//
//    i = 0;
//    flag = 0;
//    while (*str)
//    {
//        mir = 0;
//        if (*str == '\\' && (str += 2))
//            mir = 1;
//        if (!flag && *str == br && !mir && (flag += 1))
//            i++;
//        else if (flag && *str == rb && !mir && (flag -= 1))
//            i--;
//        str++;
//    }
//    return (flag ? 0 : 1);
//}
//
//short   brackets_closed(char *str)
//{
//    if (ft_strchr(str, '('))
//        if (!count_objects(str, '(', ')'))
//            return (0);
//    if (ft_strchr(str, '{'))
//    {
//        if (!count_objects(str, '{', '}'))
//            return (0);
//    }
//    return (1);
//}
//
//short   quotes_closed(char *str)
//{
//    if (ft_strchr(str, '\''))
//        if (!count_objects(str, '\'', '\''))
//            return (0);
//    if (ft_strchr(str, '"'))
//    {
//        if (!count_objects(str, '"', '"'))
//            return (0);
//    }
//    if (ft_strchr(str, '`'))
//    {
//        if (!count_objects(str, '`', '`'))
//            return (0);
//    }
//    return (1);
//}
//
//short   pipes_closed(char *str)
//{
//    if (*(str + ft_strlen(str) - 1) == '|' && *(str + ft_strlen(str) - 2) != '\\')
//        return (0);
//    return (1);
//}

//short   input_finished(char *str, t_stx **tree, short path)
//{
//    short i;
//    short id;
//
//    if (path >= 0)
//    {
//        if (!brackets_closed(str))
//            return (-1);
////        if (!seps_check(str))
////            return (unexpected_token());
//        if (!quotes_closed(str))
//            return (-1);
////        if (!input_closed(str))
////            return (-1);
//    }
//    i = find_token(tree, str);
//    id = 1;
//    if (i == 3)
//        id = script_input(str);
//    return (id ? i : -1);
//}