/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_options_conds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:08:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/06 22:58:30 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include <stdlib.h>

int				opt_is_flend(const char *arg)
{
	return (ft_strequ(arg, "--"));
}

int				opt_is_opsfl(const char *arg)
{
	return (ft_strequ(arg, "-o"));
}

int				opt_is_libfl(const char *arg)
{
	return (ft_strequ(arg, "-l"));
}

int				opt_is_null(const char *arg)
{
	return (arg == NULL);
}

void			et_rm_str(void *s, size_t l)
{
	l = 0;
	if (s)
		free(s);
}
