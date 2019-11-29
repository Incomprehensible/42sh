/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_metabranch.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:40:31 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_stx	*init_scripts(t_stx *tree)
{
	char	*scrp;
	char	**scrps;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

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

t_stx	*init_hedoc(t_stx *tree)
{
	char	*hed;
	char	**heds;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

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

t_stx	*init_math(t_stx *tree)
{
	char	*mat;
	char	**mats;
	short	i;
	t_stx	*tmp;
	t_stx	*start;

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
