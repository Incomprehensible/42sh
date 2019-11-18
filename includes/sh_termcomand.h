/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_termcomand.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 13:43:30 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 13:38:13 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TERMCOMAND_H
# define SH_TERMCOMAND_H

#include <termcap.h>
#include <stdio.h>

// Up: \ u001b [{n} A перемещает курсор вверх на n
// Вниз: \ u001b [{n} B перемещает курсор вниз на n
// Справа: \ u001b [{n} C перемещает курсор вправо на n
// Влево: \ u001b [{n} D перемещает курсор влево на n
// Следующая строка: \ u001b [{n} E перемещает курсор в начало строки на n строк вниз
// Предыдущая строка: \ u001b [{n} F] перемещает курсор в начало строки на n строк вниз
// Установить столбец: \ u001b [{n} G перемещает курсор в столбец n
// Установить позицию: \ u001b [{n}; {m} H перемещает курсор в строку n столбца m
// Очистить экран: \ u001b [{n} J очищает экран
// n = 0 очищает от курсора до конца экрана,
// n = 1 очищает от курсора до начала экрана
// n = 2 очищает весь экран
// Очистить строку: \ u001b [{n} K очищает текущую строку
// n = 0 очищает от курсора до конца строки
// n = 1 очищает курсор от начала строки
// n = 2 очищает всю строку
// Сохранить позицию: \ u001b [{s} сохраняет текущую позицию курсора
// Сохранить позицию: \ u001b [{u} восстанавливает курсор в последнюю сохраненную позицию

#define NAME "42sh$ "

/* arrow keys */
#define LEFT	"D"
#define RIGHT	"C"
#define UP		"A"
#define DOWN	"B"

#define DELCHR	"\x001b[P"
#define SAVECAR	"\x001b[s"
#define LOADCAR "\x001b[u"
#define NEXTLIN "\x001b[1E"
#define MOVEBGN "\x001b[10000D"
/* clears from cursor to end of line */
#define CLEARL	"\x001b[0J"
/* clears from cursor to start of line */
#define CLRCRBG "\x001b[1K"
/* clears all screen */
#define CLRSCR	"\x001b[2J"
#define MOVEUP	"\x001b[1A"
#define MOVEDWN "\x001b[1B"

#define BAKSP	127
#define DEL		126
#define TAB		9
#define ESC		27


#endif