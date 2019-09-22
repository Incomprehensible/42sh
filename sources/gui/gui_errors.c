/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 16:19:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/22 18:05:44 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
#include "dstring.h"

DSTRING		*dstr_nerr(char *src)
{
	DSTRING	*new;

	new = dstr_new(src);
	if (new == NULL)
		sys_fatal_memerr(sys_get_std_message(8));
	return (new);
}

DSTRING		*dstr_serr(DSTRING *src, ssize_t bi, ssize_t ei)
{
	DSTRING	*new;

	new = dstr_slice(src, bi, ei);
	if (new == NULL)
		sys_fatal_memerr(sys_get_std_message(8));
	return (new);
}

DSTRING		*dstr_scerr(DSTRING **src, ssize_t bi, ssize_t ei)
{
	DSTRING	*new;

	new = dstr_scerr(src, bi, ei);
	if (new == NULL)
		sys_fatal_memerr(sys_get_std_message(8));
	return (new);
}

DSTRING		*check_null(DSTRING *line)
{
	if (line == NULL)
		sys_fatal_memerr(sys_get_std_message(8));
	return (line);
}
