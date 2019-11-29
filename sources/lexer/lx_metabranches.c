/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_metabranches.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/28 21:46:57 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_stx	*init_dquotes(t_stx *tree)
{
	tree = (t_stx *)malloc(sizeof(t_stx));
	tree->meta = ft_strdup("\"w\"");
	tree->prev = NULL;
	tree->next = NULL;
	return (tree);
}

t_stx	*init_apofs(t_stx *tree)
{
	tree = (t_stx *)malloc(sizeof(t_stx));
	tree->meta = ft_strdup("'w'");
	tree->prev = NULL;
	tree->next = NULL;
	return (tree);
}

t_stx	*init_envar(t_stx *tree)
{
	char	*env;
	char	**envs;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

	i = 0;
	env = "~= ~+= ~-=";
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

t_stx	*init_redir(t_stx *tree)
{
	char	*red;
	char	**reds;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

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

t_stx	*init_func(t_stx *tree)
{
	char	*fun;
	char	**funs;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

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
