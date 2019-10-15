/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 12:40:32 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 20:03:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bltn.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int		ft_isodigit(int ch)
{
	if ('0' <= ch && ch <= '8')
		return (1);
	return (0);
}

static void		echo_put_och(char *arg, size_t *i)
{
	int			num;

	num = (arg[*i] - '0') * 64 + \
	(arg[*i + 1] - '0') * 8 + (arg[*i + 2] - '0');
	if (num <= 0177 && num >= 0 )
	{
		ft_putchar((char)num);
		*i += 2;
	}
}

static void		echo_put_ch(char *arg, size_t *i)
{
	int		num;

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

static void		echo_put_estr(char **args)
{
	size_t		i;
	char		ch;
	size_t		j;
	int			fle;

	i = 0;
	fle = 0;
	while (args[++i])
	{
		j = -1;
		fle != 0 ? ft_putstr(" ") : 0;
		if (!fle && args[i][0] == '-')
			continue ;
		else
			fle = 1;
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

int				echo_put_str(char **args)
{
	size_t		j;
	size_t		i;

	i = 0;
	j = 0;
	while (args[++i])
	{
		j != 0 ? ft_putstr(" ") : 0;
		if (!j && args[i][0] == '-')
			continue ;
		else
			j = 1;
		ft_putstr(args[i]);
	}
	return (0);
}

int				bltn_echo(char **args, ENV *envr)
{
	char		*flags;
	char		ch;

	if (write(STDOUT_FILENO, 0, 0) < 0)
	{
		sys_error_handler("echo: write error", E_BADFD, envr);
		return (1);
	}
	flags = ft_parse_flags(args);
	if (flags && ft_strchr(flags, 'e'))
		echo_put_estr(args);
	else
		echo_put_str(args);
	if (!flags || !ft_strchr(flags, 'n'))
		ft_putstr("\n");
	return (0);
}
