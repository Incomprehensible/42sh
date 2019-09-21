/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_darr_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 08:03:45 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:05:31 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

void			help_correct_namedir_buf(t_darr *darrcopy, DSTRING **name_fil,\
					DSTRING **name_dir)
{
	free_darr_n(darrcopy->strings, darrcopy->count);
	dstr_del(name_dir);
	dstr_del(name_fil);
}
