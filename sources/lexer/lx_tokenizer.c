/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 18:51:07 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*g_last_input;

void	global_init(void)
{
	parse_err = -1;
	syntax_err = -1;
	input_not_over = -1;
}

short	get_tokens(char *str, t_dlist **token_list)
{
	static t_stx	*tree[13];
	t_tk_type		choice;
	short			code;

	code = 0;
	if (!tree[0])
		tree_init(tree);
	global_init();
	if (!(*str) || (code = input_finished(str)) != 1)
		return (code);
	while (*str)
	{
		choice = find_token(tree, str);
		if (!(str = block_pass(choice, str, token_list, tree)))
			return (clear_tokens(token_list, 1));
	}
	if (token_list[0]->content && !back_ps_check(token_list[0]))
		return (clear_tokens(token_list, 1));
	if (token_list[0]->content && !(list_ready_to_go(token_list)))
		return (clear_tokens(token_list, 1));
	make_token(token_list, NULL, TK_EOF);
	return (1);
}

t_dlist	**toklst_init(t_dlist **token_list)
{
	if (token_list[0])
		return (token_list);
	token_list[0] = (t_dlist *)malloc(sizeof(t_dlist));
	token_list[0]->next = NULL;
	token_list[0]->content = NULL;
	token_list[0]->prev = NULL;
	token_list[0]->size = 0;
	token_list[1] = token_list[0];
	return (token_list);
}

short	concatenate_str(char **last_input, char *str)
{
	char		*tmp;

	if (!input_no_over_code())
	{
		tmp = *last_input;
		*last_input = ft_strjoin(*last_input, "\n");
		free(tmp);
	}
	tmp = *last_input;
	*last_input = ft_strjoin(*last_input, str);
	free(tmp);
	return (1);
}

// short	concatenate_str(char **last_input, char *str)
// {
// 	char		*tmp;
// 	char		last;
// 	char		prev;
// 	size_t		len;

// 	len = ft_strlen(*last_input);
// 	last = *(*last_input + (len - 1));
// 	if (len >= 2)
// 		prev = *(*last_input + (len - 2));
// 	if (last == ')' && prev == last)
// 		last ^= last;
// 	if (last != '(' && last != ')' && !(is_sep_no_space(*(skip_spaces(str)))))
// 	{
// 		tmp = *last_input;
// 		*last_input = ft_strjoin(*last_input, "\n");
// 		free(tmp);
// 	}
// 	tmp = *last_input;
// 	*last_input = ft_strjoin(*last_input, str);
// 	free(tmp);
// 	return (1);
// }

short	sh_tokenizer(char *str, t_dlist **token_list)
{
	extern char	*g_last_input;
	char		*tmp;
	short		i;

	if (!str || !(*str))
		return (0);
	token_list = toklst_init(token_list);
	if (g_last_input && concatenate_str(&g_last_input, str))
		tmp = g_last_input;
	else
		tmp = str;
	if ((i = get_tokens(tmp, token_list)) != 1)
	{
		if (i < 0 && g_last_input)
			g_last_input = null_last_input(&g_last_input);
		if (!i)
			g_last_input = g_last_input ? g_last_input : ft_strdup(tmp);
		return (i ? -1 : 0);
	}
	if (g_last_input)
		g_last_input = null_last_input(&g_last_input);
	return (1);
}
