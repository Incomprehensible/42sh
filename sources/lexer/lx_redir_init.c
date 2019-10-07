/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_redir_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/07 03:57:01 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_graph  *redir_in(void);

static t_graph    *prof_in(void)
{
    static t_graph *prof;

    if (prof)
        return (prof);
    prof = (t_graph *)malloc((sizeof(t_graph)));
    prof->type = TK_PROF_OUT;
    prof->patt = "<(z)";
    prof->forward = redir_one();
    prof->left = redir_two();
    prof->right = NULL;
    prof->next = (t_graph *)malloc((sizeof(t_graph)));
    prof->next->type = TK_PROF_IN;
    prof->next->patt = ">(z)";
    prof->next->forward = prof->forward;
    prof->next->left = prof->left;
    prof->next->right = prof->right;
    prof->next->next = NULL;
    return (prof);
}

static t_graph    *fd_in(void)
{
    static t_graph *red;
    short i;
    static t_graph *start;
    char **redir;

    if (start)
        return (start);
    i = 0;
    redir = ft_strsplit("@123456789@ @123456789@- -", ' ');
    red = (t_graph *)malloc(sizeof(t_graph));
    red->patt = redir[i];
    start = red;
    red->type = TK_FD;
    red->forward = redir_one();
    red->left = redir_two();
    red->right = redir_in();
    while (redir[++i])
    {
        red->next = (t_graph *)malloc(sizeof(t_graph));
        red->next->forward = NULL;
        red->next->left = NULL;
        red->next->right = NULL;
        red->next->next = NULL;
        red = red->next;
        red->patt = redir[i];
        red->type = CLOSE;
    }
    return (start);
}

static t_graph    *filename_in(void)
{
    static t_graph *red;

    if (red)
        return (red);
    red = (t_graph *)malloc((sizeof(t_graph)));
    red->type = TK_FILENAME;
    red->patt = "~ ";
    red->forward = fd_in();
    red->left = redir_one();
    red->right = redir_two();
    red->next = NULL;
    return (red);
}

t_graph    *redir_two(void)
{
    t_graph *red;
    static t_graph *start;
    char **redir;
    size_t r[4] = {TK_RD_A, TK_RD_RW, TK_RD_W, TK_RD_R};
    short i;

    if (start)
        return (start);
    i = 0;
    redir = ft_strsplit(">> <> > <", ' ');
    red = (t_graph *)malloc(sizeof(t_graph));
    red->patt = redir[i];
    start = red;
    red->type = r[i];
    red->forward = prof_in();
    red->left = filename_in();
    red->right = NULL;
    while (redir[++i])
    {
        red->next = (t_graph *)malloc(sizeof(t_graph));
        red->next->forward = red->forward;
        red->next->left = red->left;
        red->next->right = red->right;
        red->next->next = NULL;
        red = red->next;
        red->patt = redir[i];
        red->type = r[i];
    }
    return (start);
}

t_graph  *redir_one(void)
{
    t_graph *red;
    static t_graph *start;
    char **redir;
    size_t r[6] = {TK_RD_A, TK_RD_RW, TK_RD_W, TK_RD_R, TK_RD_R, TK_RD_W};
    short i;

    if (start)
        return (start);
    i = 0;
    redir = ft_strsplit(">>& <>& >& <& &> &<", ' ');
    red = (t_graph *)malloc(sizeof(t_graph));
    red->patt = redir[i];
    start = red;
    red->type = r[i];
    red->forward = prof_in();
    red->left = fd_in();
    red->right = filename_in();
    while (redir[++i])
    {
        red->next = (t_graph *)malloc(sizeof(t_graph));
        red->next->forward = red->forward;
        red->next->left = red->left;
        red->next->right = red->right;
        red->next->next = NULL;
        red = red->next;
        red->patt = redir[i];
        red->type = r[i];
    }
    return (start);
}

static void     comm_init(t_graph *red)
{
    red->type = TK_EXPR;
    red->patt = "~";
    red->forward = fd_in();
    red->left = redir_one();
    red->right = redir_two();
    red->next = NULL;
}

t_graph  *redir_in(void)
{
    static t_graph *red;

    if (red)
        return (red);
    red = (t_graph *)malloc((sizeof(t_graph)));
    comm_init(red);
    return (red);
}