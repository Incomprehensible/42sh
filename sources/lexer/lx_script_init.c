/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:45:47 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static t_graph	*thentk_init(void)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_THEN;
	script->patt = "then_";
	script->forward = exprtk2_init();
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static t_graph	*mathtk_init(void)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = MATH_NT;
	script->patt = NULL;
	script->forward = thentk_init();
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static t_graph	*exprtk1_init(void)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = EX;
	script->patt = "~;";
	script->forward = thentk_init();
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static void		iftk_init(t_graph *script)
{
	script->type = TK_IF;
	script->patt = "if_";
	script->forward = mathtk_init();
	script->left = exprtk1_init();
	script->right = NULL;
	script->next = NULL;
}

t_graph			*if_script_in(void)
{
	t_graph	*script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	iftk_init(script);
	return (script);
}
