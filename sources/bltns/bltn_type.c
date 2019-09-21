/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 16:14:37 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/21 20:37:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

static int	bltn_type_check_bltn(char *str)
{
	int		i;

	i = -1;
	while (bltns_str[++i])
		if (ft_strequ(bltns_str[i], str))
			return (1);
	return (0);
}

static void	bltn_type_output(char *str, int type)
{
	ft_putstr(str);
	ft_putchar(' ');

	if (type == -1)
		ft_putendl("not found");
	else if (type == 1)
		ft_putendl("is a 42shell builtin");
	else if (type == 2)
		ft_putendl("is a reserved word");
}

int			bltn_type(char **args, ENV *env)
{
	// int		i;

	// i = 0;
	// while (args[++i])
	// {
	// 	if (bltn_type_check_bltn(args[i]))
	// 		bltn_type_output(args[i], 1);
	// 	else if (bltn_type_check_bins(args[i]))
	// 		bltn_type_output(args[i], 2);
	// 	else if (bltn_type_check_alias(args[i]))
	// 		bltn_type_output(args[i], 3);
	// }
	return (0);
}