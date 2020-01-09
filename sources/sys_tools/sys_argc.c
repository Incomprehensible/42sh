/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_argc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 03:41:31 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/13 22:39:55 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rms.h"

#include "stdio.h"

static size_t	size_of_lfs(char **argv)
{
	size_t	i;
	size_t	j;
	size_t	lf;

	lf = 0;
	i = 0;
	while (argv[i])
	{
		j = 0;
		if (argv[i][j++] == '-' && argv[i][j++] == '-' && argv[i][j])
			++lf;
		++i;
	}
	return (lf);
}

static size_t	size_of_fs(char **argv)
{
	size_t	i;
	size_t	j;
	size_t	f;

	f = 0;
	i = 0;
	while (argv[i])
	{
		j = 0;
		if (argv[i][j++] == '-' && argv[i][j] && argv[i][j] != '-')
		{
			while (argv[i][j])
			{
				if (argv[i][j] == '-')
				{
					++j;
					continue ;
				}
				++f;
				++j;
			}
		}
		++i;
	}
	return (f);
}

static int		parse_lfs(char **argv, size_t size, char ***long_flags)
{
	size_t	i;
	size_t	j;
	size_t	lf;

	lf = 0;
	i = 0;
	if (!(*long_flags = ft_memalloc(sizeof(char *) * (size + 1))))
		return (1);
	while (argv[i])
	{
		j = 0;
		if (argv[i][j++] == '-' && argv[i][j++] == '-' && argv[i][j])
		{
			if (!((*long_flags)[lf] = ft_strdup(&(argv[i][j]))))
			{
				et_rm_warr(*long_flags);
				return (1);
			}
			++lf;
		}
		++i;
	}
	return (0);
}

static int		parse_fs(char **argv, size_t size, char **flags)
{
	size_t	i;
	size_t	j;
	size_t	f;

	f = 0;
	i = -1;
	if (!(*flags = ft_memalloc(sizeof(char) * (size + 1))))
		return (1);
	while (argv[++i])
	{
		j = 0;
		if (argv[i][j++] == '-' && argv[i][j] && argv[i][j] != '-')
		{
			while (argv[i][j])
			{
				if (argv[i][j] == '-')
				{
					++j;
					continue ;
				}
				(*flags)[f++] = argv[i][j++];
			}
		}
	}
	return (0);
}

/*
** SETS *LONG_FLAGS as a words array of --arguments
** and SETS *FLAGS as string of -fl-ag -s ("flags\0")
** RETURN 0 if everything ok, or 1 if ERROR
*/

int				sys_argv_parse(char **argv, \
char ***long_flags, char **flags)
{
	size_t	lf;
	size_t	f;
	int		status;

	status = 0;
	if (long_flags)
	{
		lf = size_of_lfs(argv);
		if (lf)
			status = parse_lfs(argv, lf, long_flags);
		else
			*long_flags = 0;
	}
	if (flags)
	{
		f = size_of_fs(argv);
		if (f)
			status = parse_fs(argv, f, flags) || status;
		else
			*flags = 0;
	}
	return (status);
}
