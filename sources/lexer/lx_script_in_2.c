/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_in_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:47:04 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static void		untiltk_init(t_graph *script)
{
	script->type = TK_UNTIL;
	script->patt = "until_";
	script->forward = forexpr_init(script, TK_UNTIL);
	script->left = condition_init(script);
	script->right = NULL;
	script->next = NULL;
}

t_graph			*until_script_in(void)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	untiltk_init(script);
	return (script);
}

static t_graph	*elsetk_init(t_graph *expr)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_ELSE;
	script->patt = "else_";
	script->forward = expr;
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static t_graph	*fitk_init(void)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_FI;
	script->patt = "fi_";
	script->forward = sep_init();
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

t_graph			*exprtk2_init(void)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_EXPR;
	script->patt = "~;";
	script->forward = fitk_init();
	script->left = elsetk_init(script);
	script->right = NULL;
	script->next = NULL;
	return (script);
}
