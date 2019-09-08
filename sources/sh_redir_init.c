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

//???
//recreate the system with closing fd
static t_graph    *close_in(void)
{
    t_graph *red;

    red = (t_graph *)malloc((sizeof(t_graph)));
    red->type = fd_tk;
    red->patt = "-";
    red->forward = NULL;
    red->left = NULL;
    red->right = NULL;
    red->next = NULL;
    return (red);
}

static t_graph    *fd_in(void)
{
    static t_graph *red;

    if (red)
        return (red);
    red = (t_graph *)malloc((sizeof(t_graph)));
    red->type = fd_tk;
    red->patt = "@123456789@";
    red->forward = redir_one();
    red->left = redir_two();
    red->right = close_in();
    red->next = (t_graph *)malloc((sizeof(t_graph)));
    red->next->type = fd_tk;
    red->next->patt = "@123456789@-";
    red->next->forward = red->forward;
    red->next->left = red->left;
    red->next->right = red->right;
    red->next->next = NULL;
    return (red);
}

static t_graph    *filename_in(void)
{
    static t_graph *red;

    if (red)
        return (red);
    red = (t_graph *)malloc((sizeof(t_graph)));
    red->type = filename_tk;
    red->patt = "~";
    red->forward = fd_in();
    red->left = NULL;
    red->right = NULL;
    red->next = NULL;
    return (red);
}

t_graph    *redir_two(void)
{
    t_graph *red;
    static t_graph *start;
    char **redir;
    int r[4] = {rd_w_tk, rd_r_tk, rd_a_tk, rd_rw_tk};
    short i;

    if (start)
        return (start);
    i = 0;
    redir = ft_strsplit("> < >> <>", ' ');
    red = (t_graph *)malloc(sizeof(t_graph));
    red->patt = redir[i];
    start = red;
    red->type = r[i];
    red->forward = filename_in();
    red->left = NULL;
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
    int r[6] = {rd_w_tk, rd_r_tk, rd_a_tk, rd_rw_tk, rd_r_tk, rd_rw_tk};
    short i;

    if (start)
        return (start);
    i = 0;
    redir = ft_strsplit(">& <& >>& <>&, &> &<", ' ');
    red = (t_graph *)malloc(sizeof(t_graph));
    red->patt = redir[i];
    start = red;
    red->type = r[i];
    red->forward = filename_in();
    red->left = fd_in();
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

static void     comm_init(t_graph *red)
{
    red->type = expr_tk;
    red->patt = "~";
    red->forward = redir_one();
    red->left = redir_two();
    red->right = NULL;
    red->next = NULL;
}

t_graph  *redir_in(void)
{
    t_graph *red;

    red = (t_graph *)malloc((sizeof(t_graph)));
    comm_init(red);
    return (red);
}