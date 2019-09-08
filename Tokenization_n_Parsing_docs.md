# TOKENS DOCUMENTATION (En/Rus)
Структура токена состоит из его типа и его строкового значения. Токен классифицирует лексическое значение подстрок(слов) или какого то символа в полученной строке ввода пользователя. Токены лежат в двусвязнном списке. После создания списка токенов, данный список уходит на исполнение и парсинг командным интерпритатором.

# ПОЯСНЕНИЯ К ДОКУМЕНТАЦИИ
Токен в пояснениях к токенам изображается в виде текстовой конструкции `token_type:"token_value" => next_token`.  
Иногда для сокращения цепочки токенов типа `TK_EXPR` будут сжиматься в `TK_EXPR:"tval1 tval2 tval3 ..."`.  
Промежуточные `emty_tk` будут сокращены из `=> emty_tk =>` в `=>=>`.

# Tokens Types
1. `TK_EMPTY` - empty token for separations of args, or something else.
1. `TK_EXPR` - EXPRESSION token for cmds, args, builtns or something like that.
1. `TK_SEP` - ';' - separation token.
1. `TK_OR` - '||' - or operator token.
1. `TK_AND` - '&&' - and operator token.
1. `TK_BCKR_PS` - '&' - background procces token.
1. `TK_RD_W` - '> >&' redirection token for write.
1. `TK_RD_R` - '< <&' redirection token for read.
1. `TK_RD_A` - '>>' redirection token for appending-write.
1. `TK_RD_RW` - '<>' redirection token for read_n_write.
1. `TK_HERED, TK_WORD` - '<< WORD' here-document tokens.
1. `TK_FD, TK_FILENAME` - file_descriptor token and file_name token for redirections.
1. `TK_PIPE` - pipe token.
1. `TK_ASSIGM` - '= +=' assigment operator token for setting variables.
1. `TK_NAME, TK_VALUE` - '[varname] = [value]', tokens for vars.
1. `TK_MATH` - '(()), let' arephmetic operator token.
1. `TK_SUBSH` - '()' sub-shell token.
1. `TK_DEREF` - '$' dereferencing token.
1. `TK_IF, TK_THEN, TK_ELSE, TK_FI, TK_WHILE, TK_DO, TK_DONE, fTK_OR, TK_IN, TK_UNTIL` - flow_instructions tokens.
1. `TK_EXEC, TK_FUNCTION` - exec and function tokens.
1. `TK_APOF, TK_DQUOTE` - [ ', " ] - double-quotes and single-quotes tokens.
1. `TK_EOF` - end_of_file(input) token.
1. `TK_FEND` - end of the function body.
1. `TK_VAR` - '{}' variable engine token.
1. ~~`newln_tk` - newline token.~~
# Expression:

Выражением являются сущность принимающая в себя аргументы и возвращающая свой статус работы.  

>`ls -lR /`  
>преобразуется в:  
>**`TK_EXPR:"ls" => TK_EMPTY => TK_EXPR:"-lR" => TK_EMPTY => TK_EXPR:"/"`**  
  
Выражением собирается из последовательности токенов TK_EXPR.  
Обычно выражение соотвестует исполняемой программе вызываемой командной оболочкой, втроенной функцией командной оболочки, внешней функцией командной оболочки или просто константой или подобной сущностью.  

# Separators:

Разделителями считаются символы разделяющие исполнение выражений или присваиваний. К таким ключевым символам относятся  `[';', '&&', '||']`.  
Разделитель-конвейр (pipe), относится к особому типу разделителя и поэтому вынесен в отдельную группу.

>`echo abcd; echo a`  
>преобразуется в:  
>**`TK_EXPR:"echo" => TK_EMPTY => TK_EXPR:"abcd" => TK_SEP => TK_EXPR:"echo" => exmpty_tk => TK_EXPR:"a" => TK_EOF`**  
  
>`echo abcd || echo a`  
>преобразуется в:  
>**`TK_EXPR:"echo" => => TK_EXPR:"abcd" => TK_OR => TK_EXPR:"echo" => => TK_EXPR:"a" => TK_EOF`**  
  
Логические алгоритмы действуют несколько иначе. Проводится оптимизация вычисления И ИЛИ и для ИЛИ будет достаточно успешно выполненой команды в одном экземляре, наоборот И, для остановки выполнения команд достаточно одного неверно завершившегося выражения.  
Все выражения выполняются синхронно при любом из разделителей данной группы.  

# Pipes:

Конвеер-разделитель или труба используется для перенаправления потока вывода из одного выражения в поток ввода другого выражения. Все инструкции в конвеере исполняются асинхронно.<!-- ! НЕИЗВЕСТНО КАК МЫ БУДЕМ РАБОТАТЬ СО ВСТРОЕННЫМИ ФУНКЦИЯМИ, ВСЕГДА ЗАПУСКАТЬ ИХ КАК ФОРК ИЛИ ВСЕ ТАКИ СИНХРОННО ОБРАБАТЫВАТЬ ВСТРОЕННЫЕ КОМАНДЫ -->  

> !! смотреть комментарий внутри  

# Redirections:

Перенаправление ввода-вывода действует только на одну инструкцию (цельное выражение), кроме тех случаев когда они объявлены явно через exec.  
Есть несколько типов перенаправления, подробнее о них вы можете узнать из документации Bash.  
Приводим примеры токенизации строки с перенаправлениями потоков.

>`echo abc dca > test.txt`  
>становится:  
>` TK_EXPR:"echo abc dca" =>=> rd_w =>=> TK_FILENAME:"test.txt";`

>`cat -e <test.txt`  
>становится:  
>`TK_EXPR:"cat -e" =>=> TK_RD_W => TK_FILENAME:"test.txtw"`

>`echo abc dca 5> 77`  
>становится:  
>` TK_EXPR:"echo abc dca" =>=> TK_FD:"5" => rd_w =>=> TK_FILENAME:"77";`

>`echo abc dca 5>& 77`  
>становится:  
>` TK_EXPR:"echo abc dca" =>=> TK_FD:"5" => rd_w =>=> TK_FD:"77";`

Перенаправления совершаются в порядке слева направо.

# Assigment:

Пример токенизации присвающего оператора:

>`VARNAME=VALUE; VARNAME+=VALUE`  
>становится:  
>`TK_NAME:"VARNAME" => TK_ASSIGM:"=" => TK_VALUE:"VALUE" => TK_SEP => TK_NAME:"VARNAME" => TK_ASSIGM:"+" => TK_VALUE:"VALUE"`

>`VARNAME = VALUE`  
>становится:  
>`TK_EXPR:"VARNAME" =>=> TK_EXPR:"=" =>=> TK_EXPR:"VALUE"`

# Math:

Математический оператор заключается в двойные круглые скобки `((MATH))` или объявляется с помощью `let "MATH"`.

>`((1 + 2 + 3))`  
>становится:  
>`TK_MATH:"1 + 2 + 3"`

>`let "1 + 2 + 3"`  
>становится:  
>`TK_MATH:"1 + 2 + 3"`

>`$((1 + 2 + 3))`  
>становится:  
>`TK_DEREF => TK_MATH:"1 + 2 + 3"`

# Subshell:

Исполнение команд в "подоболочке" происходит в отдельном процессе командной оболочке, все переменные объявленные внутри не переносятся в родительский процесс (нашу оболочку) и все изменения файл-дескрипторов и прочего остается в "подоболочке". Скорее всего будет реализована как обычный `execve()` или продвинутый `fork()`.

>`(cmd1 cmd1; cmd2 cmd2)`  
>становится:  
>`TK_SUBSH:"cmd1 cmd1; cmd2 cmd2"`

>`$(cmd1)`  
>становится:  
>`TK_DEREF => TK_SUBSH:"cmd1"`

# Flow instructions:

Ключевые конструкции языка (циклы, условия) токенизируются напрямую как набор токенов ключевой конструкции и выражения и прочие.

	if expr1;
	then
		expr2;
		expr3;
	else
		expr4;
		expr5;
	fi;

>Реализуется как:  
>`TK_IF =>=> TK_EXPR:"expr1" => TK_SEP =>=> TK_THEN =>=> exprt_tk:"expr2" => TK_SEP => .... => TK_FI`

*Новая линия может быть представлена отдельным токеном равно значным разделителю.*

# Functions:

Функции в баш похожи на выражения, так что их вызов ничем не будет отличаться от исполнения программы или встроенной функции в bash. Для объявления функции используется отдельный токен и отдельные инструкции интерпритатора.

Образец токенизации:


	function fname {cmd1; cmd2; cmd3}

>или

	fname() {cmd1; cmd2; cmd3}

>Реализуется как:  
>`TK_FUNCTION =>=> TK_NAME:"fname: =>=> TK_EXPR:"cmd1" => TK_SEP => TK_EXPR:"cmd2: => ... => TK_FEND`

# Newline:

Токен новой линии по сути своей равен сепаратору команд.

# Apostroph and Double-quotes

Токены апострофа и двойных ковычек реализуют один expression в итоге. Одинарные ковычки экраннируют что угодно, а двойные не экранинуют смивол '$'  b '\'.

>`ls "a b c $HOME"`  
>становится:  
>`TK_EXPR:"ls" =>=> TK_DQUOTE => TK_EXPR:"a b c " => deref_rk => TK_NAME:"HOME" => TK_DQUOTE`

>**ВАРИАНТ ВРЕМЕННЫЙ**
