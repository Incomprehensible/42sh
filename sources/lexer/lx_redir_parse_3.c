/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_parse_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/11 06:49:11 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

void			merge_name(char **first, char *sec)
{
	char	*tmp;

	tmp = ft_strjoin(*first, sec);
	free(*first);
	//free(*sec);
	*first = tmp;
}

void			merge_into_name(t_dlist *token_list, t_dlist **tok)
{
	t_dlist	*next;
	t_dlist	*current;

	while (token_list)
	{
		if (token_list->content && (TOK_TYPE == TK_NAME))
		{
			current = token_list;
			token_list = token_list->next;
			if (token_list && (TOK_TYPE == TK_NAME))
			{
				merge_name(&((t_tok *)(current->content))->value, TOK_VALUE);
				next = token_list->next;
				token_list->next = NULL;
				del_tokens(token_list);
				tok[1] = next ? tok[1] : current;
				current->next = next;
				token_list = current;
			}
		}
		else
			token_list = token_list->next;
	}
}

static void		into_filename(t_dlist *last_token, t_dlist **tok)
{
	t_dlist	*token_list;

	token_list = tok[1];
	while (token_list && token_list != last_token)
		token_list = token_list->prev;
	if (!token_list)
		return ;
	token_list = token_list->next;
	while (token_list && (TOK_TYPE == TK_DEREF || TOK_TYPE == TK_EXPR))
	{
		if (token_list && TOK_TYPE == TK_DEREF)
			token_list = (token_list->next) ? token_list->next : token_list;
		else if (token_list && TOK_TYPE == TK_EXPR)
			TOK_TYPE = TK_FILENAME;
		token_list = token_list->next;
	}
}

static char		*parse_filename(char *s, size_t i, t_stx **tr, t_dlist **tok)
{
	t_dlist *last_token;
	char	*new;

	last_token = tok[1];
	new = pull_token(s - i, i);
	if (!(parse_comm(new, tok, tr, 0)))
	{
		free(new);
		return (NULL);
	}
	free(new);
	into_filename(last_token, tok);
	return (s);
}

char			*filename_pull(char *s, t_stx **tr, t_dlist **tok)
{
	int		flag;
	size_t	i;

	flag = 1;
	i = 0;
	while (*s && !is_redir(s) && !(flag && (ft_isspace(*s) ||
	is_sep_no_space(*s))))
	{
		if (*s == '\'' || *s == '"' || *s == '\\' || *s == '(')
			flag = flag ? 0 : 1;
		++s;
		i++;
	}
	if (i)
		return (parse_filename(s, i, tr, tok));
	return (s);
}
