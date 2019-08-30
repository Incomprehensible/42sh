/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:52:49 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 10:29:07 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int				parse_brackets(const char *str, const char ch)
{
	int i;
	int fl;
	int otr;

	i = -1;
	fl = 0;
	otr = 0;
	while (str[++i] != ']' && str[i] != '\0')
	{
		if (str[i] == '!')
			otr = 1;
		if (str[i] == ch && (fl = 1))
			break ;
		if (str[i] == '-' && ch >= str[i - 1] && ch <= str[i + 1] && (fl = 1))
			break ;
	}
	if ((fl && !otr) || (!fl && otr))
	{
		while (str[i] != ']')
			++i;
		return (i + 1);
	}
	return (-1);
}

int				nmatch(const char *s1, const char *s2)
{
	int		step;

	if ((*s1 != '\0') && (*s2 == '*') && (*s2 + 1 != '\\'))
		return (nmatch(s1 + 1, s2) || nmatch(s1, s2 + 1));
	else if ((*s1 == *s2 && *s2 != '\0') || (*s2 == '?' && *s2 + 1 != '\\'))
		return (nmatch(s1 + 1, s2 + 1));
	else if (*s2 == '[' && *s2 + 1 != '\\')
	{
		if ((step = parse_brackets(s2, *s1)) != -1)
			return (nmatch(s1 + 1, s2 + step));
		return (0);
	}
	else if (*s1 == '\0' && *s2 == '\0')
		return (1);
	else if (*s1 == '\0' && *s2 == '*' && (*s2 + 1 != '\\'))
		return (nmatch(s1, s2 + 1));
	return (0);
}
