# 42SH ARIPHMETIC STANDARTS

((var = 4, var = ++var, var = var + var + ++var, var += (var + (var * 3 / 4)+6+7-5 ) ))

```
input:
"var = 4, var = ++var, var = var + var + ++var, var += (var + (var * 3 / 4)+6+7-5 ) " :: string
return:
long (52)
```

`long		ariphmetic_eval(char *expressions, ENV *env, int *err);`
* expressions: Строка с выражениями разделенными символом `,`
* env: Специальная переменная окружения
* err: Переменная для записи статуса ошибок

Возвращаемое значение: В случае отсутствия ошибок в выражениях, вернусь результат последнего выражения отделенного `запятой`.

## EXAMPLES

```
IN: a = 45 + 3
RET: 48

IN: a = 5, a += 6, ++a
RET: 12

ENV: a="STRING"
IN: ++a
RET: ERR: TEXT OSHIBKI NA SVOE YSMOTRENIE
COM: Код ошибки записывается в int *err

ENV: a=
IN: ++a
RET: 1

ENV: a=-100
IN: --a
RET: -101

IN: 45 + 45 -
RET: ERR: TEXT OSHIBKI NA SVOE YSMOTRENIE

IN: a++
COM: ЛИБО ОБРАБАТЫВАЕМ КАК ОШИБКУ, ЛИБО КАК ОБЫЧНЫЙ ПРЕФИКСНЫЙ, ЛИБО КАК КРУТЫЕ ЧУВАКИ

IN: 0xffAc
RET: 65452

IN: 0b1110002
RET: ERR: STROKA ERROR

IN: 0777
RET: 511

ENV: a=45
IN: -a
RET: -45

IN: -0xff
RET: -255

IN: +5
RET: 5

```

## FUNNY COMMENTS

BASES OF NUMBERS:
* `0xhhhhhh` - 0x[0-9a-fA-F]
* `0bhhhhhh` - 0x[0-1]
* `0ohhhhhh` - 0o[0-7]
* `0hhhhhhh` - 0[0-7]
* `hhhhhhhh` - [0-9]

OPERATIONS:
* `++, --, !, ~, -`
* `<<, >>`
* `&, |, ^`
* `*, /, %`
* `+, -`
* `==, !=, >=, <=, >, <`
* `&&, ||`

EXAMPLES OF PARSING:
```
EVAL:
( 8 + 2 * 5 ) / ( 1 + 3 * 2 — 4 )
FIN:
8 2 5 * + 1 3 2 * + 4 - /

EVAL:
2 * ++3 & 4
FIN:
2 3 ++ * 4 &

EVAL:
a && b || !(c + d)
FIN:
a b c d + ! || &&

EVAL:
!(c + d) || a && b 
FIN: c d + ! a b && || 

EVAL:
! a && b
FIN: a ! b &&

EVAL:
1 + 2 - 4 * (4 + 3 / 4) >> 5
FIN:
1 2 4 4 3 4 / + 5 >> * - +
```
