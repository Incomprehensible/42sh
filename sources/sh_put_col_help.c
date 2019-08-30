/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_put_col_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 08:15:58 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:27:37 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_termcomand.h"

ushort	get_col(const int lencol)
{
	struct winsize		term;

	ioctl(0, TIOCGWINSZ, &term);
	return (term.ws_col / lencol);
}
