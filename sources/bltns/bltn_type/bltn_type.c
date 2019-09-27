/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 16:14:37 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/27 17:54:58 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_type.h"

static int	bltn_type_check_bltn(char *str)
{
	int		i;

	i = -1;
	while (bltns_str[++i])
		if (ft_strequ(bltns_str[i], str))
			return (1);
	return (0);
}

static int	bltn_type_check_reserved(char *str)
{
	int		i;

	i = -1;
	while (bltn_keywords[++i])
		if (ft_strequ(bltn_keywords[i], str))
			return (1);
	return (0);
}

void		bltn_type_output(char *str, int type, DSTRING *val)
{
	ft_putstr(str);
	ft_putchar(' ');
	if (type == -1)
		ft_putendl("not found");
	else if (type == 1)
		ft_putendl("is a 42shell builtin");
	else if (type == 2)
		ft_putendl("is a reserved word");
	else if (type == 3)
	{
		ft_putstr("is an alias for ");
		ft_putendl(val->txt);
	}
	else if (type == 4)
	{
		ft_putstr("is ");
		ft_putendl(val->txt);
	}
}

int			bltn_type(char **args, ENV *env)
{
	int		i;
	DSTRING	*val;

	i = 0;
	val = NULL;
	while (args[++i])
	{
		if (val)
			dstr_del(&val);
		if (bltn_type_check_bltn(args[i]))
			bltn_type_output(args[i], 1, NULL);
		else if (bltn_type_check_reserved(args[i]))
			bltn_type_output(args[i], 2, NULL);
		else if (bltn_type_check_alias(args[i], &val, env))
			bltn_type_output(args[i], 3, val);
		else if (bltn_type_check_bins(args[i], env, &val))
			bltn_type_output(args[i], 4, val);
		else
			bltn_type_output(args[i], -1, NULL);
	}
	if (val)
		dstr_del(&val);
	return (0);
}
