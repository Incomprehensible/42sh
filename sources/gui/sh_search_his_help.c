/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_search_his_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:01:14 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/21 15:11:45 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

int			i(int c, int i)
{
	if (i >= c)
		return (i % c);
	return (i);
}

void		col_search_his(t_darr overlap, t_darr his, DSTRING *strd)
{
	if (overlap.count == 0 && his.count != 0)
		put_col(his, strd);
	else if (overlap.count != 0)
		put_col(overlap, strd);
}

void		direction_help(t_darr o, t_darr his, t_indch *ich, DSTRING **strd)
{
	if ((ich->ind == 0 || ich->ch == 0x12) && o.strings[0])
		dstr_cutins_dstr(strd, o.strings[i(o.count, ich->ind++)], LENSERCH - 1);
	if (ich->ch == TAB)
		col_search_his(o, his, (*strd));
}
