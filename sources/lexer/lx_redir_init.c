/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 20:08:20 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static t_graph	*filename_in(void)
{
	static t_graph	*red;

	if (red)
		return (red);
	red = (t_graph *)malloc((sizeof(t_graph)));
	red->type = TK_FILENAME;
	red->patt = ft_strdup("~ ");
	red->forward = fd_in();
	red->left = redir_one();
	red->right = redir_two();
	red->next = NULL;
	return (red);
}

t_graph			*redir_two(void)
{
	t_graph			*red;
	static t_graph	*start;
	char			**redir;
	size_t const	r[] = {TK_RD_R, TK_RD_W, TK_RD_A, \
	TK_RD_RW, TK_RD_W, TK_RD_R};

	if (start)
		return (start);
	redir = ft_strsplit("&> &< >> <> > <", ' ');
	red = (t_graph *)malloc(sizeof(t_graph));
	red->patt = ft_strdup(redir[0]);
	start = red;
	red->type = r[0];
	red->forward = prof_in();
	red->left = filename_in();
	red->right = NULL;
	construct_node(red, redir, r);
	ft_arrmemdel((void **)redir);
	return (start);
}

t_graph			*redir_one(void)
{
	t_graph			*red;
	static t_graph	*start;
	char			**redir;
	size_t const	r[] = {TK_RD_A, TK_RD_RW, TK_RD_W, TK_RD_R};

	if (start)
		return (start);
	redir = ft_strsplit(">>& <>& >& <&", ' ');
	red = (t_graph *)malloc(sizeof(t_graph));
	red->patt = ft_strdup(redir[0]);
	start = red;
	red->type = r[0];
	red->forward = prof_in();
	red->left = fd_in();
	red->right = filename_in();
	construct_node(red, redir, r);
	ft_arrmemdel((void **)redir);
	return (start);
}

static void		comm_init(t_graph *red)
{
	red->type = TK_EXPR;
	red->patt = ft_strdup("~");
	red->forward = fd_in();
	red->left = redir_one();
	red->right = redir_two();
	red->next = NULL;
}

t_graph			*redir_in(void)
{
	static t_graph	*red;

	if (red)
		return (red);
	red = (t_graph *)malloc((sizeof(t_graph)));
	comm_init(red);
	return (red);
}
