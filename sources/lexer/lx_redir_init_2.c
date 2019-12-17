/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_init_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/17 18:16:17 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

void		construct_node(t_graph *g, char **dict, size_t *r)
{
	short	i;

	i = 0;
	while (dict[++i])
	{
		g->next = (t_graph *)malloc(sizeof(t_graph));
		g->next->forward = g->forward;
		g->next->left = g->left;
		g->next->right = g->right;
		g->next->next = NULL;
		g = g->next;
		g->patt = ft_strdup(dict[i]);
		g->type = r ? r[i] : CLOSE;
	}
}

t_graph		*prof_in(void)
{
	static t_graph	*prof;

	if (prof)
		return (prof);
	prof = (t_graph *)malloc((sizeof(t_graph)));
	prof->type = TK_PROF_OUT;
	prof->patt = ft_strdup("<(z)");
	prof->forward = redir_one();
	prof->left = redir_two();
	prof->right = redir_in();
	prof->next = (t_graph *)malloc((sizeof(t_graph)));
	prof->next->type = TK_PROF_IN;
	prof->next->patt = ft_strdup(">(z)");
	prof->next->forward = prof->forward;
	prof->next->left = prof->left;
	prof->next->right = prof->right;
	prof->next->next = NULL;
	return (prof);
}

t_graph		*fd_in(void)
{
	static t_graph	*red;
	static t_graph	*start;
	char			**redir;

	if (start)
		return (start);
	redir = ft_strsplit("@0123456789@ @0123456789@- -", ' ');
	red = (t_graph *)malloc(sizeof(t_graph));
	red->patt = ft_strdup(redir[0]);
	start = red;
	red->type = TK_FD;
	red->forward = redir_one();
	red->left = redir_two();
	red->right = redir_in();
	construct_node(red, redir, NULL);
	ft_arrmemdel((void **)redir);
	return (start);
}
