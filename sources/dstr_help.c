/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dstr_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:47:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 07:54:28 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"
#include "sh_readline.h"

ssize_t			sh_dstr_iscmd(const DSTRING *str)
{
	ssize_t		i;
	ssize_t		space;
	ssize_t		smcln;

	space = dstrrchr(str, ' ');
	smcln = dstrrchr(str, ';');
	if (smcln != -1 && (i = smcln + 1))
	{
		while (ft_isspace(str->txt[i]))
			i++;
		if (i >= space + 1)
			return (-1);
	}
	i = 0;
	while (str->txt[i] == ' ')
		++i;
	if (str->strlen == 0 || space == -1 || space + 1 == i)
		return (-1);
	return (space + 1);
}
