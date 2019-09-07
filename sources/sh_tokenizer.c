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

//убери лишние аргументы из функций
//check if last element in linked list is eof, rather then if [0] and [1] are equal
void    make_token(t_dlist **list, char *value, int type)
{
    t_tok           *token_data;
    t_dlist         *tmp;

    token_data = (t_tok *)malloc(sizeof(t_tok));
    if (token_data)
    {
        token_data->value = value;
        token_data->type = type;
        if (list[0] == list[1] && !list[0]->token)
        {
            list[1]->token = token_data;
            list[0]->next = NULL;
        }
        else if (list[0] == list[1])
        {
            tmp = list[0];
            tmp->next = ft_dlstnew(NULL, 0);
            tmp->next->token = token_data;
            tmp->next->next = NULL;
            tmp->next->prev = list[0];
            list[1] = tmp->next;
        }
        else
        {
            tmp = list[1];
            tmp->next = ft_dlstnew(NULL, 0);
            tmp->next->token = token_data;
            tmp->next->next = NULL;
            tmp->next->prev = tmp;
            list[1] = tmp->next;
        }
    }
}

int     layer_parse_one(char *meta, char *str, char *end)
{
    char *tmp;
    char *tmp2;

    tmp2 = meta;
    while (str && str != end && *str && *meta)
    {
        while (*str && *meta && (*str == *meta || special_meta(*meta)) && str != end)
        {
            tmp = str;
//            str++;
//            meta++;
            if (*meta && special_meta(*meta))
            {
                str = process_reg(str, meta, end);
                if (!str)
                    return (0);
                if (*meta == '@' || *meta == '!' || *meta == '.')
                    meta = get_point(meta);
                meta++;
            }
            else
            {
                if (*str != *meta)
                {
                    str = tmp;
                    meta = tmp2;
                    break;
                }
                str++;
                meta++;
            }
            if (!*meta)
                return (1);
//            if (*str != *meta)
//            {
//                str = tmp;
//                meta = tmp2;
//                break;
//            }
        }
        if (str == end && *meta == *end)
            return (1);
        str++;
    }
    return (0);
}

//we probably don't need parse_sep and parse_eof
char    *block_pass(short i, char *str, t_dlist **toklst, t_stx **tree)
{
    static char*    (*ptr[12])(char *, t_dlist **, t_stx **, short);

    ptr[0] = &parse_mirror;
    ptr[1] = &parse_scripts;
//    ptr[2] = &parse_hedoc;
    ptr[3] = &parse_math;
    ptr[4] = &parse_quotes;
    ptr[5] = &parse_envar;
    ptr[6] = &parse_redir;
    ptr[7] = &parse_func;
    ptr[8] = &parse_subsh;
    ptr[9] = &parse_comm;
//    ptr[10] = &parse_eof;
    str = ptr[i](str, toklst, tree, 0);
    return (str);
}

int     check_branch(char *str, t_stx *tree)
{
    int choice;
    char *end;

    choice = 0;
    if (*str == '\'')
        return (choice);
    //}?
    if (!(end = ft_strchr(str, '}')))
        end = ft_strchr(str, ';');
    if (!end)
        if (!(end = ft_strchr(str, '\n')))
            end = str + ft_strlen(str);
    while (tree && !choice)
    {
        choice = layer_parse_one(tree->meta, str, end);
        tree = tree->next;
    }
    return (choice);
    //here we return id whether we have match or not
}

//what do we do if the first token is sep ? we may just skip it
int     find_token(t_stx **tree, char *str)
{
    int i;

    i = 0;
    while (tree[i] && !check_branch(str, tree[i]))
        i++;
    return (tree[i] ? i : 9);
}

//there is problem! if we have working first block and then the block which is not finished, we can't detect
//any unfinished input. Cause our find block func searchs for the first occurance of block
//we must check for unfinished input every time the new block is processed.
short    get_tokens(char *str, t_dlist **token_list)
{
//    t_dlist         *token;
//    t_tok           *token_data;
    static t_stx    *tree[12];
    short           choice;

    if (!tree[0])
        tree_init(tree);
//    token = token_list[0];
//    token_data = (t_tok *)malloc(sizeof(t_tok));
//    token_data->type = 0;
//    token_data->value = NULL;
//    token->token = token_data;
//    if (!input_finished(str, tree))
//        return (0);
    //while (token_list[1]->token->type != eof_tk && *str)
    while (*str)
    {
        if ((choice = input_finished(str, tree)) == -1)
            return (0);
        str = block_pass(choice, str, token_list, tree);
//        str = block_pass(find_token(tree, str), str, token_list, tree);
    }
    make_token(token_list, NULL, eof_tk);
    return (1);
}

t_dlist    **toklst_init(t_dlist **token_list)
{
    if (token_list[0])
        return (token_list);
    token_list[0] = (t_dlist *)malloc(sizeof(t_dlist));
    token_list[0]->next = NULL;
    token_list[0]->content = NULL;
    token_list[0]->prev = NULL;
    token_list[0]->size = 0;
    token_list[0]->token = NULL;
    token_list[1] = token_list[0];
    return (token_list);
}

int				sh_tokenizer(char *str, t_dlist **token_list, char **env)
{
    static char *last_input;
    char        *tmp;

    token_list = toklst_init(token_list);
    if (last_input)
    {
        tmp = last_input;
        last_input = ft_strjoin(last_input, str);
        free(tmp);
        tmp = last_input;
    }
    else
        tmp = str;
    if (!get_tokens(tmp, token_list))
    {
        if (last_input)
        {
            tmp = last_input;
            ft_strjoin(last_input, str);
            free(tmp);
        }
        else
            last_input = ft_strdup(str);
    }
    return (1);
    //return (end_of_input(token_list[1]) ? 1 : 0);
}

