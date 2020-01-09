/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_cleaners.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/14 19:32:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

void			*ft_arrmemdel(void **ap)
{
	unsigned char	**ptr;
	int				i;
	int				j;

	i = 0;
	j = 0;
	ptr = (unsigned char **)ap;
	while (ptr[j])
	{
		free(ptr[j]);
		j++;
	}
	free(ap);
	return (NULL);
}

static t_dlist	*rm_void_token(t_dlist *token_list)
{
	t_dlist	*tmp_next;
	t_dlist	*tmp_prev;

	tmp_next = token_list->next;
	tmp_prev = token_list->prev;
	token_list->next = NULL;
	del_tokens(token_list);
	tmp_prev->next = tmp_next;
	tmp_next->prev = tmp_next ? tmp_prev : tmp_next->prev;
	token_list = tmp_prev;
	return (token_list);
}

static void		get_rid_of_void(t_dlist *token_list)
{
	while (token_list)
	{
		if (TOK_TYPE == TK_EMPTY)
		{
			token_list = token_list->next;
			if (token_list && TOK_TYPE == TK_EMPTY)
				token_list = rm_void_token(token_list);
			else
			{
				if (token_list)
					token_list = token_list->next;
			}
		}
		else
			token_list = token_list->next;
	}
}

static void		trim_separators(t_dlist *token_list)
{
	while (token_list)
	{
		if (TOK_TYPE == TK_SEP)
		{
			free(TOK_VALUE);
			TOK_VALUE = NULL;
		}
		token_list = token_list->next;
	}
}

short			list_ready_to_go(t_dlist **token_list)
{
	t_dlist	*start;

	exec_on(token_list[0]);
	get_rid_of_void(token_list[0]);
	merge_into_expr(token_list[0], token_list);
	if (!seps_check(token_list[0]) || !args_check(token_list[0]))
		return (0);
	trim_separators(token_list[0]);
	start = token_list[0];
	while (start->next)
		start = start->next;
	token_list[1] = start;
	return (1);
}
