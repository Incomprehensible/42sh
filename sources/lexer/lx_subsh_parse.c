/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_subsh_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/19 01:49:22 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*pull_subsh(char *str, t_dlist **tok, t_tk_type type)
{
	size_t	br;
	size_t	i;

	br = 1;
	i = 1;
	while (str[i] && br)
	{
		if (str[i] == '\'' || str[i] == '"')
			i += skip_field(&str[i], str[i]);
		else if (str[i] == '(')
			br++;
		else if (str[i] == ')')
			br--;
		if (br)
			i = (str[++i] == '\\') ? ++i : i;
	}
	make_token(tok, pull_token(str + 1, i - 1), type);
	str += i + 1;
	return (str);
}

char   *parse_subsh(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char *patt;
    char *tmp;

	ind = 0;
    patt = "(z)";
    if (*str == '\\' || *str != '(')
        return (parse_comm(str, tok, tree, '('));
    if ((tmp = reg_process(patt, TK_SUBSH, str, tok, tree)) == str)
        return (NULL);
    str = tmp;
    return (str);
}