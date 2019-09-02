/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 12:40:26 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/01 14:24:18 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

char				is_ctrl(const t_indch indch)
{
	if (indch.ch == 0x1 || indch.ch == 0x5 \
		|| indch.ch == 0x15 || indch.ch == 0x14)
		return (1);
	return (0);
}
