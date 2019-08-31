/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 13:22:09 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/31 18:44:46 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/includes/libft.h"
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;
	int	c;

	fd = open("/Users/gdaemoni/Desktop/42sh/sources/histr.txt", O_CREAT | O_RDWR | O_TRUNC, S_IREAD|S_IWRITE);
	c = -1;
	while (++c < 2550)
		ft_putendl_fd(ft_itoa(c), fd);
	close(fd);
}