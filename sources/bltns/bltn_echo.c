/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 12:40:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 18:15:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bltn.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static void		echo_put_och(char *arg, size_t *i)
{
	int			num;

	num = (arg[*i] - '0') * 64 + \
	(arg[*i + 1] - '0') * 8 + (arg[*i + 2] - '0');
	if (num <= 0177 && num >= 0)
	{
		ft_putchar((char)num);
		*i += 2;
	}
}

static void		echo_put_ch(char *arg, size_t *i)
{
	if (arg[*i] == '\\')
		ft_putchar('\\');
	else if (arg[*i] == 'a')
		ft_putchar('\a');
	else if (arg[*i] == 'b')
		ft_putchar('\b');
	else if (arg[*i] == 'f')
		ft_putchar('\f');
	else if (arg[*i] == 'n')
		ft_putchar('\n');
	else if (arg[*i] == 'r')
		ft_putchar('\r');
	else if (arg[*i] == 't')
		ft_putchar('\t');
	else if (arg[*i] == 'v')
		ft_putchar('\v');
	else if (ft_isodigit(arg[*i]) && \
	ft_isodigit(arg[*i + 1]) && ft_isodigit(arg[*i + 1]))
		echo_put_och(arg, i);
	else
		*i -= 1;
}

static void		echo_put_estr(char **args, char fl_space)
{
	size_t		i;
	size_t		j;

	i = -1;
	while (args[++i])
	{
		j = -1;
		!fl_space ? fl_space = 1 : ft_putstr(" ");
		while (args[i][++j])
			if (args[i][j] == '\\')
			{
				++j;
				echo_put_ch(args[i], &j);
			}
			else
				ft_putchar(args[i][j]);
	}
}

int				echo_flag_check(char **argc, char *flags)
{
	int		i;
	int		j;

	i = 1;
	flags[0] = 0;
	flags[1] = 1;
	flags[2] = 0;
	flags[3] = 0;
	while (argc[i] && argc[i][0] == '-')
	{
		j = 0;
		while (argc[i][++j])
		{
			if (argc[i][j] == 'e')
				flags[0] = 1;
			else if (argc[i][j] == 'n')
				flags[1] = 0;
			else
				return (i);
		}
		++i;
		flags[3] = 1;
	}
	return (i);
}

int				bltn_echo(char **args, ENV *envr)
{
	char		flags_e_n_0[4];
	int			i;

	if (write(STDOUT_FILENO, 0, 0) < 0)
	{
		sys_error_handler("echo: write error", E_BADFD, envr);
		return (1);
	}
	i = echo_flag_check(args, flags_e_n_0);
	if (flags_e_n_0[0] && flags_e_n_0[3])
		echo_put_estr(&(args[i]), flags_e_n_0[2]);
	else
	{
		while (args[i])
		{
			!flags_e_n_0[2] ? flags_e_n_0[2] = 1 : ft_putstr(" ");
			ft_putstr(args[i++]);
		}
	}
	if (flags_e_n_0[1] || !flags_e_n_0[3])
		ft_putstr("\n");
	return (0);
}
