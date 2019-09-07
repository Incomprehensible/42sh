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

//some blocks precede others - or have more priority over others. like math blocks comes before envar and we must check
//it before envar.

t_stx    *init_sep(t_stx *tree)
{
    char *sep;
    char **seps;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    sep = "; | & && ||";
    seps = ft_strsplit(sep, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(seps[i]);
    tree->prev = NULL;
    start = tree;
    while (seps[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(seps[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_mirror(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("\\");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_scripts(t_stx *tree)
{
    char *scrp;
    char **scrps;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    scrp = "if_ then_ else_ fi_ while_ do_ done_ for_ until_";
    scrps = ft_strsplit(scrp, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(scrps[i]);
    tree->prev = NULL;
    start = tree;
    while (scrps[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(scrps[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_hedoc(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("<<?");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_math(t_stx *tree)
{
    char *mat;
    char **mats;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    mat = "let_ ((~))";
    mats = ft_strsplit(mat, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(mats[i]);
    tree->prev = NULL;
    start = tree;
    while (mats[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(mats[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_quotes(t_stx *tree)
{
    char *qu;
    char **qus;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    qu = "\"~\" '~'";
    qus = ft_strsplit(qu, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(qus[i]);
    tree->prev = NULL;
    start = tree;
    while (qus[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(qus[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_envar(t_stx *tree)
{
    char *env;
    char **envs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    env = "= +=";
    envs = ft_strsplit(env, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(envs[i]);
    tree->prev = NULL;
    start = tree;
    while (envs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(envs[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_redir(t_stx *tree)
{
    char *red;
    char **reds;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    red = "@123456789@> >& &> @123456789@>& > < >> <>";
    reds = ft_strsplit(red, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(reds[i]);
    tree->prev = NULL;
    start = tree;
    while (reds[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(reds[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_func(t_stx *tree)
{
    char *fun;
    char **funs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    fun = "function_ ?(~)^{~}";
    funs = ft_strsplit(fun, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(funs[i]);
    tree->prev = NULL;
    start = tree;
    while (funs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(funs[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_subsh(t_stx *tree)
{
    char *sub;
    char **subs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    sub = "$(~) (~)";
    subs = ft_strsplit(sub, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(subs[i]);
    tree->prev = NULL;
    start = tree;
    while (subs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(subs[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_comm(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("exec_");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

//we don't access it like usual - but at the end of program
//maybe we don't need it and will have to make our own rules to check whether input is over
//like in heredoc and scripts we may request more input
t_stx    *init_eof(t_stx *tree)
{
    char *eof;
    char **eofs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    eof = "; & \n";
    eofs = ft_strsplit(eof, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(eofs[i]);
    tree->prev = NULL;
    start = tree;
    while (eofs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(eofs[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

void    tree_init(t_stx **tree)
{
    //tree[0] = init_sep(tree[0]);
    tree[0] = init_mirror(tree[0]);
    tree[1] = init_scripts(tree[1]);
    tree[2] = init_hedoc(tree[2]);
    tree[3] = init_math(tree[3]);
    tree[4] = init_quotes(tree[4]);
    tree[5] = init_envar(tree[5]);
    tree[6] = init_redir(tree[6]);
    tree[7] = init_func(tree[7]);
    tree[8] = init_subsh(tree[8]);
    tree[9] = init_comm(tree[9]);
    tree[10] = init_eof(tree[10]);
    tree[11] = NULL;
}
