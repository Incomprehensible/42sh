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

#include <stdio.h>

//static void    get_rid_of_holes(t_dlist *token_list)
//{
//    while (token_list)
//    {
//        if (TOK_TYPE == HOLE)
//            TOK_TYPE = TK_SEP;
//        token_list = token_list->prev;
//    }
//}

void    get_rid_of_holes(t_dlist *token_list)
{
    t_dlist *tmp_next;
    t_dlist *tmp_prev;

    while (token_list)
    {
        if (TOK_TYPE == HOLE)
        {
            tmp_next = token_list->next;
            tmp_prev = token_list->prev;
            free(token_list->content);
            free(token_list);
            if (tmp_prev)
                tmp_prev->next = tmp_next;
            if (tmp_next)
                tmp_next->prev = tmp_prev;
        }
        token_list = token_list->next;
    }
}

//void    get_rid_of_holes(t_dlist *token_list)
//{
//    t_dlist *tmp_next;
//    t_dlist *tmp_prev;
//
//    while (token_list)
//    {
//        if (TOK_TYPE == HOLE)
//        {
//            tmp_next = token_list->next;
//            tmp_prev = token_list->prev;
//            free(token_list->content);
//            free(token_list);
//            tmp_prev->next = tmp_next;
//            if (tmp_next)
//                tmp_next->prev = tmp_prev;
//        }
//        token_list = token_list->prev;
//    }
//}

static void    get_rid_of_void(t_dlist *token_list)
{
    t_dlist *tmp_next;
    t_dlist *tmp_prev;

    while (token_list)
    {
        if (TOK_TYPE == TK_EMPTY)
        {
            token_list = token_list->next;
            if (token_list && TOK_TYPE == TK_EMPTY)
            {
                tmp_next = token_list->next;
                tmp_prev = token_list->prev;
                if (TOK_VALUE)
                    free(TOK_VALUE);
                free(token_list->content);
                free(token_list);
                tmp_prev->next = tmp_next;
                if (tmp_next)
                    tmp_next->prev = tmp_prev;
                token_list = tmp_prev;
            }
            else
                if (token_list)
                    token_list = token_list->next;
        }
        else
            token_list = token_list->next;
    }
}

static void    exec_on(t_dlist *token_list)
{
    while (token_list)
    {
        if (TOK_TYPE == TK_EXPR)
            if (!ft_strcmp(TOK_VALUE, "exec"))
            TOK_TYPE = TK_EXEC;
        token_list = token_list->next;
    }
}

static void trim_separators(t_dlist *token_list)
{
    while (token_list)
    {
        if (TOK_TYPE == TK_SEP)
        {
            free(TOK_VALUE);
            TOK_VALUE = NULL;
        }
        token_list = token_list->next;
    }

}

short    list_ready_to_go(t_dlist **token_list)
{
    t_dlist *start;

    get_rid_of_holes(token_list[0]);
//    if (!seps_check(token_list[0]))
//        return (0);
    get_rid_of_void(token_list[0]);
    exec_on(token_list[0]);
    trim_separators(token_list[0]);
    start = token_list[0];
    while (start->next)
        start = start->next;
    token_list[1] = start;
//    printf("value %s type %zx\n", ((t_tok *)(token_list[1]->content))->value, ((t_tok *)(token_list[1]->content))->type);
//    while (token_list[0])
//    {
//        printf("value %s type %zx\n", ((t_tok *)(token_list[0]->content))->value, ((t_tok *)(token_list[0]->content))->type);
//        token_list[0] = token_list[0]->next;
//    }
    if (!seps_check(token_list[0]))
        return (0);
    return (1);
}