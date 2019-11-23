/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_in_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:48:19 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static t_graph	*prexpr_init(t_graph *tk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = GLUE;
	script->patt = NULL;
	script->forward = do_init(tk);
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static t_graph	*in_init(t_graph *fortk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_IN;
	script->patt = "in^";
	script->forward = prexpr_init(fortk);
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static t_graph	*inexpr_init(t_graph *fortk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_VAR;
	script->patt = "!in";
	script->forward = in_init(fortk);
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static void		fortk_init(t_graph *script)
{
	script->type = TK_FOR;
	script->patt = "for_";
	script->left = inexpr_init(script);
	script->forward = forexpr_init(script, TK_FOR);
	script->right = NULL;
	script->next = NULL;
}

t_graph			*for_script_in(void)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	fortk_init(script);
	return (script);
}
