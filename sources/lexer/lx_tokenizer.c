/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/19 13:56:11 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

//убери лишние аргументы из функций
void    make_token(t_dlist **list, char *value, t_tk_type type)
{
    t_tok           token_data;
    t_dlist         *tmp;

	token_data.value = value;
	token_data.type = type;
	if (list[0] == list[1] && !list[0]->content)
	{
		list[1]->content = (t_tok *)malloc(sizeof(t_tok));
		*((t_tok *)(list[1]->content)) = token_data;
		list[0]->next = NULL;
	}
	else if (list[0] == list[1])
	{
		tmp = list[0];
		tmp->next = ft_dlstnew(NULL, 0);
		tmp->next->content = (t_tok *)malloc(sizeof(t_tok));
		*((t_tok *)(tmp->next->content)) = token_data;
		tmp->next->next = NULL;
		tmp->next->prev = list[0];
		list[1] = tmp->next;
	}
	else
	{
		tmp = list[1];
		tmp->next = ft_dlstnew(NULL, 0);
		tmp = tmp->next;
		tmp->content = (t_tok *)malloc(sizeof(t_tok));
		*((t_tok *)(tmp->content)) = token_data;
		tmp->next = NULL;
		tmp->prev = list[1];
		list[1] = tmp;
	}
}

int     layer_parse_one(char *meta, char *str)
{
    while (*str && *meta && !(is_sep_no_space(*str) && *meta != 'w') && (*str == *meta || special_meta(*meta)))
    {
        if (*meta && special_meta(*meta))
        {
            if (!(str = process_reg(str, meta)))
                return (0);
            if (*meta == '@' || *meta == '!' || *meta == '.')
                meta = get_point(meta);
            meta++;
        }
        else
        {
            str++;
            meta++;
        }
    }
    if (!*meta || (*meta == '_' && is_sep_no_space(*str)))
        return (1);
    return (0);
}

char    *block_pass(short i, char *str, t_dlist **tok, t_stx **tree)
{
    static char*    (*ptr[13])(char *, t_dlist **, t_stx **, short);

    if (!ptr[0])
    {
        ptr[0] = &parse_dquotes;
        ptr[1] = &parse_apofs;
        ptr[2] = &parse_math;
        ptr[3] = &parse_subsh;
        ptr[4] = &parse_scripts;
        ptr[5] = &parse_envar;
        ptr[6] = &parse_hedoc;
        ptr[7] = &parse_proc;
        ptr[8] = &parse_redir;
        ptr[9] = &parse_func;
        ptr[10] = &parse_lambda;
        ptr[11] = &parse_comm;
        ptr[12] = &parse_deref;
    }
    if (i == EMPTY)
        return (parse_empty(str, 0x0, tok));
	else if (i == COMMENT)
		return (skip_comment(str));
    else if (i == SEPS)
        return (parse_sep(str, tok, 0));
    str = ptr[i](str, tok, tree, 0);
    return (str);
}

int     check_branch(char *str, t_stx *tree)
{
    short choice;

    choice = 0;
    while (!choice && *str && tree)
    {
        choice = layer_parse_one(tree->meta, str);
        tree = tree->next;
    }
    return (choice);
}

short     find_token(t_stx **tree, char *str)
{
    t_tk_type i;
    short   choice;

    i = 0;
    choice = 0;
    if (ft_isspace(*str) || *str == '\n')
        return (EMPTY);
    else if (*str == '$' && valid_deref(str))
        return (DEREF);
    else if (is_sep_no_space(*str) && *str && !is_redir(str + 1))
        return (SEPS);
    while (*str && !(is_sep_no_space(*str) && !is_redir(str + 1)) && !choice)
    {
        i = 0;
        if (*str == '$' || *str == '\\')
            return (EXPRS);
		if (*str == '#')
			return (COMMENT);
        while (tree[i] && !(choice = check_branch(str, tree[i])))
            i++;
        if (!tree[i])
            str++;
    }
    return (tree[i] ? i : EXPRS);
}

extern void			DBG_PRINT_TOKENS(t_dlist *toklst);

short   clear_tokens(t_dlist **tokens, short id)
{
    t_dlist *token_list;

	if (id)
    	unexpected_token(tokens);
	// DBG_PRINT_TOKENS(*tokens);
    while (*tokens)
    {
        token_list = *tokens;
        *tokens = (*tokens)->next;
        if (token_list->content && TOK_VALUE)
            free(TOK_VALUE);
        free(token_list->content);
        free(token_list);
    }
    tokens[0] = NULL;
    tokens[1] = NULL;
    return (-1);
}

void    global_init(void)
{
    PARSE_ERR = -1;
    SYNTAX_ERR = -1;
    INPUT_NOT_OVER = -1;
}

short    get_tokens(char *str, t_dlist **token_list)
{
    static t_stx    *tree[13];
    t_tk_type       choice;
    short           code;

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

t_dlist    **toklst_init(t_dlist **token_list)
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

void    concatenate_str(char **last_input, char *str)
{
    char        *tmp;
	char		last;

    // tmp = *last_input;
	last = *(*last_input + (ft_strlen(*last_input) - 1));
	if (last != '(' && last != ')')
	{
		tmp = *last_input;
		*last_input = ft_strjoin(*last_input, "\n");
    	free(tmp);
	}
    // *last_input = ft_strjoin(*last_input, "\n");
    // free(tmp);
    tmp = *last_input;
    *last_input = ft_strjoin(*last_input, str);
    free(tmp);
}

short				sh_tokenizer(char *str, t_dlist **token_list)
{
    static char *last_input;
	char        *tmp;
    short       i;

	if (!str || !(*str))
		return (0);
    token_list = toklst_init(token_list);
    if (last_input)
    {
        concatenate_str(&last_input, str);
        tmp = last_input;
    }
    else
        tmp = str;
    if ((i = get_tokens(tmp, token_list)) != 1)
    {
		if (i < 0 && last_input)
		{
			free(last_input);
			last_input = NULL;
		}
        // if (i < 0 && last_input)
        //     free(last_input);
		//was last_input = i ? NULL : ft_strdup(tmp);
		if (!i)
        	last_input = last_input ? last_input : ft_strdup(tmp);
        return (i ? -1 : 0);
    }
    if (last_input)
    {
        free(last_input);
        last_input = NULL;
    }
    return (1);
}

