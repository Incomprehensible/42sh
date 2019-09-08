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

char*   parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i)
{
    str = parse_empty(str, "", tok);
    if (*str != '<')
    {
        while (*str && *str != '<')
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), expr_tk);
        i = 0;
        while (*str && *str == '<')
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), hered_tk);
        i = 0;
        while (*str && !(is_separator(*str)))
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), word_tk);
    }
    return (parse_sep(str, tok, tree, 0));
}

char        *redir_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{

}

//проверять на экранирование
//в цикле expr_tk если есть экранирование то продолжаем
//pull exec here
char*   parse_redir(char *str, t_dlist **tok, t_stx **tree, short i)
{
    static t_graph *redir;
    char *tmp;

    tmp = str;
    if (!redir)
    {
        redir = redir_in();
        //if we find > > <> redir = redir->forward;
        //else if we find &< etc redir = redir->left;
    }

    if ((str = redir_traverse(redir, str, tok, tree)) == tmp)
        return (block_pass(9, str, tok, tree));
}
