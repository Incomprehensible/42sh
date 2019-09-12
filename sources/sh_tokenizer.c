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
void    make_token(t_dlist **list, char *value, t_tk_type type)
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
        }
        if (str == end && *meta == *end)
            return (1);
        str++;
    }
    return (0);
}

char    *block_pass(t_tk_type i, char *str, t_dlist **toklst, t_stx **tree)
{
    static char*    (*ptr[9])(char *, t_dlist **, t_stx **, short);

//    ptr[0] = &parse_sep;
    ptr[0] = &parse_scripts;
    //was 4
    ptr[1] = &parse_envar;
    //was 1
    ptr[2] = &parse_hedoc;
    //+1
    ptr[3] = &parse_math;
    ptr[4] = &parse_quotes;
    //
    ptr[5] = &parse_redir;
    ptr[6] = &parse_func;
    ptr[7] = &parse_subsh;
    ptr[8] = &parse_comm;
    str = ptr[i](str, toklst, tree, 0);
    return (str);
}

int     check_branch(char *str, t_stx *tree)
{
    short choice;
    char *end;

    choice = 0;
    if (*str == '\'')
        return (choice);
    if (!(end = ft_strchr(str, ';')))
        end = ft_strchr(str, ')') + 1;
    if (!end)
        if (!(end = ft_strchr(str, '\n')))
            end = str + ft_strlen(str);
    while (tree && !choice)
    {
        choice = layer_parse_one(tree->meta, str, end);
        tree = tree->next;
    }
    return (choice);
}

//what do we do if the first token is sep ? we may just skip it
t_tk_type     find_token(t_stx **tree, char *str)
{
    t_tk_type i;

    i = 0;
    while (tree[i] && !check_branch(str, tree[i]))
        i++;
    return (tree[i] ? i : TK_EXPRS);
}

static char     *pull_word(char *str)
{
    size_t  i;

    i = 0;
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    while (*str && *str != '<')
        str++;
    while (*str && *str == '<')
        str++;
    if (!(*str))
        return (NULL);
    while (*str && !(is_separator(*str)))
    {
        str++;
        i++;
    }
    return (pull_token(str - i, i));
}

static short    get_hedoc(char *str, int id)
{
    static char *word;

    if (!id)
    {
        word = pull_word(str);
        if (!word)
            return (-1);
    }
    else
    {
        str += ft_strlen(str) - 1;
        while (*str != '<')
        {
            if (!(ft_strcmp(word, str)) && *(str - 1) != '<')
            {
                free(word);
                return (1);
            }
            str--;
        }
    }
    return (0);
}

short   clear_tokens(t_dlist **tokens)
{
    t_dlist *tmp;

    while (*tokens)
    {
        tmp = *tokens;
        *tokens = (*tokens)->next;
        free(tmp);
    }
    free(tokens);
    return (-1);
}

//there is problem! if we have working first block and then the block which is not finished, we can't detect
//any unfinished input. Cause our find block func searchs for the first occurance of block
//we must check for unfinished input every time the new block is processed.
short    get_tokens(char *str, t_dlist **token_list)
{
    static t_stx    *tree[10];
    short           choice;
    static short    flag;
    static short    path;

    if (!tree[0])
        tree_init(tree);
    while (*str)
    {
        if (flag < 0)
        {
            if (!get_hedoc(str, 1))
                return (0);
            flag = 1;
        }
        if ((choice = input_finished(str, tree, path)) == -1)
            return (0);
        path = -1;
        if (choice == 2 && flag != 1 && (flag = -1))
        {
            //check if hedoc has a closing word, otherwise throw exception
            if (get_hedoc(str, 0) < 0)
            {
                flag = 0;
                return (-1);
            }
            return (0);
        }
        //syntax error case
        if (!(str = block_pass(choice, str, token_list, tree)))
            return (clear_tokens(token_list));
    }
    path = 0;
    flag = 0;
    make_token(token_list, NULL, TK_EOF);
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

short				sh_tokenizer(char *str, t_dlist **token_list)
{
    static char *last_input;
    char        *tmp;
    short       i;

    token_list = toklst_init(token_list);
    if (last_input)
    {
        tmp = last_input;
        last_input = ft_strjoin(last_input, " ");
        free(tmp);
        tmp = last_input;
        last_input = ft_strjoin(last_input, str);
        free(tmp);
        tmp = last_input;
    }
    else
        tmp = str;
    if ((i = get_tokens(tmp, token_list)) <= 0)
    {
        if (!i && !last_input)
            last_input = ft_strdup(str);
        return (0);
//        if (last_input)
//        {
//            tmp = last_input;
//            last_input = ft_strjoin(last_input, " ");
//            free(tmp);
//            tmp = last_input;
//            last_input = ft_strjoin(last_input, str);
//            free(tmp);
//        }
//        else
//            last_input = ft_strdup(str);
    }
    if (last_input)
    {
        free(last_input);
        last_input = NULL;
    }
    return (1);
}

