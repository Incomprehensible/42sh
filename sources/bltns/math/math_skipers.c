/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_skipers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:19:59 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:20:43 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

int		skip_brackets(char *str, size_t *ind)
{
	int		br_cnt;

	br_cnt = 0;
	while (1)
	{
		if (str[*ind] == 0)
			return (0);
		else if (str[*ind] == '(')
			++br_cnt;
		else if (str[*ind] == ')')
			--br_cnt;
		++(*ind);
		if (!br_cnt)
			return (1);
	}
}

void	skip_word(char *str, size_t *ind)
{
	while (ft_isalnum(str[*ind]))
		++(*ind);
}

void	skip_alnums_n_space(char *str, size_t *ind)
{
	while (ft_isalnum(str[*ind]) || ft_isspace(str[*ind]))
		++(*ind);
}

void	skip_spaces(char *str, size_t *ind)
{
	while (ft_isspace(str[*ind]))
		++(*ind);
}

size_t	op_search(DSTRING *expr, char *op, size_t op_len)
{
	size_t		ind;
	char		*stri;
	int			i;

	ind = 0;
	stri = expr->txt;
	while (ind < expr->strlen)
	{
		skip_alnums_n_space(stri, &ind);
		if (stri[ind] == '(' && !skip_brackets(stri, &ind))
			return (0);
		if (stri[ind] == ')')
			return (0);
		if (ft_strnequ(&(stri[ind]), op, op_len))
			return (ind);
		++ind;
	}
	return (SIZE_T_MAX);
}
