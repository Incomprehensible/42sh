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

//some blocks precede others - or have more priority over others. like math blocks comes before envar and we must check
//it before envar.

//t_stx    *init_sep(t_stx *tree)
//{
//    char *sep;
//    char **seps;
//    short i;
//    t_stx *tmp;
//    t_stx *start;
//
//    i = 0;
//    sep = "; | & && ||";
//    seps = ft_strsplit(sep, ' ');
//    tree = (t_stx *)malloc(sizeof(t_stx));
//    tree->meta = seps[i];
//    tree->prev = NULL;
//    start = tree;
//    while (seps[++i])
//    {
//        tmp = tree;
//        tree->next = (t_stx *)malloc(sizeof(t_stx));
//        tree = tree->next;
//        tree->meta = seps[i];
//        tree->next = NULL;
//        tree->prev = tmp;
//    }
//    return (start);
//}

//t_stx    *init_mirror(t_stx *tree)
//{
//    tree = (t_stx *)malloc(sizeof(t_stx));
//    tree->meta = ft_strdup("\\");
//    tree->prev = NULL;
//    tree->next = NULL;
//    return (tree);
//}

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
    tree->meta = scrps[i];
    tree->prev = NULL;
    start = tree;
    while (scrps[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = scrps[i];
        tree->next = NULL;
        tree->prev = tmp;
    }
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
    tree->meta = heds[i];
    tree->prev = NULL;
    start = tree;
    while (heds[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = heds[i];
        tree->next = NULL;
        tree->prev = tmp;
    }
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
    tree->meta = mats[i];
    tree->prev = NULL;
    start = tree;
    while (mats[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = mats[i];
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_dquotes(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("\"x\"");
    tree->prev = NULL;
    tree->next = NULL;
    return (tree);
}

t_stx    *init_apofs(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("'x'");
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
    tree->meta = envs[i];
    tree->prev = NULL;
    start = tree;
    while (envs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = envs[i];
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
    tree->meta = reds[i];
    tree->prev = NULL;
    start = tree;
    while (reds[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = reds[i];
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
    fun = "function_ ?(~) ?(~)^{z}";
    funs = ft_strsplit(fun, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = funs[i];
    tree->prev = NULL;
    start = tree;
    while (funs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = funs[i];
        tree->next = NULL;
        tree->prev = tmp;
    }
    return (start);
}

t_stx    *init_lambda(t_stx *tree)
{
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = ft_strdup("{z}");
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
    proc = "<~(z) >~(z)";
    procs = ft_strsplit(proc, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = procs[i];
    tree->prev = NULL;
    start = tree;
    while (procs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = procs[i];
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
//    sub = "(z) $(z) $(~; (~;";
    sub = "(x) $(x)";
    subs = ft_strsplit(sub, ' ');
    tree = (t_stx *)malloc(sizeof(t_stx));
    tree->meta = subs[i];
    tree->prev = NULL;
    start = tree;
    while (subs[++i])
    {
        tmp = tree;
        tree->next = (t_stx *)malloc(sizeof(t_stx));
        tree = tree->next;
        tree->meta = subs[i];
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
//t_stx    *init_eof(t_stx *tree)
//{
//    char *eof;
//    char **eofs;
//    short i;
//    t_stx *tmp;
//    t_stx *start;
//
//    i = 0;
//    eof = "; & \n";
//    eofs = ft_strsplit(eof, ' ');
//    tree = (t_stx *)malloc(sizeof(t_stx));
//    tree->meta = eofs[i];
//    tree->prev = NULL;
//    start = tree;
//    while (eofs[++i])
//    {
//        tmp = tree;
//        tree->next = (t_stx *)malloc(sizeof(t_stx));
//        tree = tree->next;
//        tree->meta = eofs[i];
//        tree->next = NULL;
//        tree->prev = tmp;
//    }
//    return (start);
//}

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
