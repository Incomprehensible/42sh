/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_termcomand.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 13:43:30 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/27 22:55:10 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TERMCOMAND_H
# define SH_TERMCOMAND_H

# include <termcap.h>
# include <stdio.h>

# define LEFT	"D"
# define RIGHT	"C"
# define UP		"A"
# define DOWN	"B"
# define MOVEUP	"\x001b[1A"
# define SAVECAR "\x001b[s"
# define LOADCAR "\x001b[u"
# define NEXTLIN "\x001b[1E"
# define MOVEBGN "\x001b[10000D"
# define CLEARL	"\x001b[0J"
# define CLRSCR	"\x001b[2J"
# define BAKSP	127
# define DEL	126
# define TAB	9
# define ESC	27

#endif
