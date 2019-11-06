/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_metatree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/06 04:59:00 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_stx    *init_scripts(t_stx *tree)
{
    char *scrp;
    char **scrps;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    scrp = "if_ then_ else_ fi_ while_ do_ done_ for_ until_ break_ continue_";
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
	ft_arrmemdel((void **)scrps);
    return (start);
}

t_stx    *init_hedoc(t_stx *tree)
{
    char *hed;
    char **heds;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    hed = "<< <<<";
    heds = ft_strsplit(hed, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(heds[i]);
    tree->prev = NULL;
    start = tree;
    while (heds[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(heds[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
	ft_arrmemdel((void **)heds);
    return (start);
}

t_stx    *init_math(t_stx *tree)
{
    char *mat;
    char **mats;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    mat = "let_ ((";
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
	ft_arrmemdel((void **)mats);
    return (start);
}

t_stx    *init_dquotes(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("\"w\"");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_apofs(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("'w'");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_envar(t_stx *tree)
{
    char *env;
    char **envs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    env = "= += -=";
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
	ft_arrmemdel((void **)envs);
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
	ft_arrmemdel((void **)reds);
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
    fun = "function_ ?(~) ?(~)^{z}";
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
	ft_arrmemdel((void **)funs);
    return (start);
}

t_stx    *init_lambda(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("{w}");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_proc(t_stx *tree)
{
    char *proc;
    char **procs;
    short i;
    t_stx *tmp;
    t_stx *start;

    i = 0;
    proc = "<~(w) >~(w)";
    procs = ft_strsplit(proc, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup(procs[i]);
    tree->prev = NULL;
    start = tree;
    while (procs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = ft_strdup(procs[i]);
        tree->next = NULL;
        tree->prev = tmp;
    }
	ft_arrmemdel((void **)procs);
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
    sub = "(w) $(w)";
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
	ft_arrmemdel((void **)subs);
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

void    tree_init(t_stx **tree)
{
    tree[0] = init_dquotes(tree[0]);
    tree[1] = init_apofs(tree[0]);
    tree[2] = init_math(tree[1]);
    tree[3] = init_subsh(tree[2]);
    tree[4] = init_scripts(tree[3]);
    tree[5] = init_envar(tree[4]);
    tree[6] = init_hedoc(tree[5]);
    tree[7] = init_proc(tree[6]);
    tree[8] = init_redir(tree[7]);
    tree[9] = init_func(tree[8]);
    tree[10] = init_lambda(tree[9]);
    tree[11] = init_comm(tree[10]);
    tree[12] = NULL;
}
