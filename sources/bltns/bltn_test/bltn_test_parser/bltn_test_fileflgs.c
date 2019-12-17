/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test_fileflgs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 14:33:59 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/17 16:06:24 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"

static int	bltn_test_chflghlp(t_flags_file *flgs, char c)
{
	if (c == 'r')
		flgs->f_read = 1;
	else if (c == 's')
		flgs->f_notnull = 1;
	else if (c == 'S')
		flgs->f_sckt = 1;
	else if (c == 'u')
		flgs->f_suid = 1;
	else if (c == 'w')
		flgs->f_write = 1;
	else if (c == 'x')
		flgs->f_exec = 1;
	else if (c == 'n')
		flgs->s_notnull = 1;
	else if (c == 'z')
		flgs->s_null = 1;
	else
		return (0);
	return (1);
}

static int	bltn_test_checkflgs(t_flags_file *flgs, char c)
{
	if (c == 'b')
		flgs->f_blckdev = 1;
	else if (c == 'c')
		flgs->f_symbdev = 1;
	else if (c == 'd')
		flgs->f_dir = 1;
	else if (c == 'e')
		flgs->f_exst = 1;
	else if (c == 'f')
		flgs->f_file = 1;
	else if (c == 'g')
		flgs->f_segid = 1;
	else if (c == 'h')
		flgs->f_symb = 1;
	else if (c == 'k')
		flgs->f_stbit = 1;
	else if (c == 'L')
		flgs->f_symb = 1;
	else if (c == 'p')
		flgs->f_chnl = 1;
	else
		return (bltn_test_chflghlp(flgs, c));
	return (1);
}

int			bltn_test_prsfileflgs(t_flags_file *flgs, char *str)
{
	int	fl;
	int	i;

	i = 0;
	fl = 0;
	if (str[0] != '-')
		return (-1);
	while (str[++i])
	{
		if (bltn_test_checkflgs(flgs, str[i]) == 1)
		{
			if (fl == 1)
				return (-2);
			fl = 1;
		}
		else
			return (-1);
	}
	return (1);
}
