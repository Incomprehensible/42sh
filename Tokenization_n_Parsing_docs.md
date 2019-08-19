# TOKENS DOCUMENTATION (En/Rus)
Структура токена состоит из его типа и его строкового значения. Токен классифицирует лексическое значение подстрок(слов) или какого то символа в полученной строке ввода пользователя. Токены лежат в двусвязнном списке. После создания списка токенов, данный список уходит на исполнение и парсинг командным интерпритатором.

# ПОЯСНЕНИЯ К ДОКУМЕНТАЦИИ
Токен в пояснениях к токенам изображается в виде текстовой конструкции `token_type:"token_value" => next_token`.  
Иногда для сокращения цепочки токенов типа `expr_tk` будут сжиматься в `expr_tk:"tval1 tval2 tval3 ..."`.  
Промежуточные `emty_tk` будут сокращены из `=> emty_tk =>` в `=>=>`.

# Tokens Types
1. `empty_tk` - empty token for separations of args, or something else.
1. `expr_tk` - EXPRESSION token for cmds, args, builtns or something like that.
1. `sep_tk` - ';' - separation token.
1. `or_tk` - '||' - or operator token.
1. `and_tk` - '&&' - and operator token.
1. `bckr_ps_tk` - '&' - background procces token.
1. `rd_w_tk` - '> >&' redirection token for write.
1. `rd_r_tk` - '< <&' redirection token for read.
1. `rd_a_tk` - '>>' redirection token for appending-write.
1. `rd_rw_tk` - '<>' redirection token for read_n_write.
1. `hered_tk, word_tk` - '<< WORD' here-document tokens.
1. `fd_tk, filename_tk` - file_descriptor token and file_name token for redirections.
1. `pipe_tk` - pipe token.
1. `assigm_tk` - '= +=' assigment operator token for setting variables.
1. `name_tk, value_tk` - '[varname] = [value]', tokens for vars.
1. `math_tk` - '(()), let' arephmetic operator token.
1. `subsh_tk` - '()' sub-shell token.
1. `deref_tk` - '$' dereferencing token.
1. `if_tk, then_tk, else_tk, fi_tk, while_tk, do_tk, done_tk, for_tk, in_tk, until_tk` - flow_instructions tokens.
1. `exec_tk, function_tk` - exec and function tokens.
1. `apof_tk, dquote_tk` - [ ', " ] - double-quotes and single-quotes tokens.
1. `eof_tk` - end_of_file(input) token.
1. `fend_tk` - end of the function body.
1. `var_tk` - '{}' variable engine token.
1. ~~`newln_tk` - newline token.~~
# Expression:

Выражением являются сущность принимающая в себя аргументы и возвращающая свой статус работы.  

>`ls -lR /`  
>преобразуется в:  
>**`expr_tk:"ls" => empty_tk => expr_tk:"-lR" => empty_tk => expr_tk:"/"`**  
  
Выражением собирается из последовательности токенов expr_tk.  
Обычно выражение соотвестует исполняемой программе вызываемой командной оболочкой, втроенной функцией командной оболочки, внешней функцией командной оболочки или просто константой или подобной сущностью.  

# Separators:

Разделителями считаются символы разделяющие исполнение выражений или присваиваний. К таким ключевым символам относятся  `[';', '&&', '||']`.  
Разделитель-конвейр (pipe), относится к особому типу разделителя и поэтому вынесен в отдельную группу.

>`echo abcd; echo a`  
>преобразуется в:  
>**`expr_tk:"echo" => empty_tk => expr_tk:"abcd" => sep_tk => expr_tk:"echo" => exmpty_tk => expr_tk:"a" => eof_tk`**  
  
>`echo abcd || echo a`  
>преобразуется в:  
>**`expr_tk:"echo" => => expr_tk:"abcd" => or_tk => expr_tk:"echo" => => expr_tk:"a" => eof_tk`**  
  
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
>` expr_tk:"echo abc dca" =>=> rd_w =>=> filename_tk:"test.txt";`

>`cat -e <test.txt`  
>становится:  
>`expr_tk:"cat -e" =>=> rd_w_tk => filename_tk:"test.txtw"`

>`echo abc dca 5> 77`  
>становится:  
>` expr_tk:"echo abc dca" =>=> fd_tk:"5" => rd_w =>=> filename_tk:"77";`

>`echo abc dca 5>& 77`  
>становится:  
>` expr_tk:"echo abc dca" =>=> fd_tk:"5" => rd_w =>=> fd_tk:"77";`

Перенаправления совершаются в порядке слева направо.

# Assigment:

Пример токенизации присвающего оператора:

>`VARNAME=VALUE; VARNAME+=VALUE`  
>становится:  
>`name_tk:"VARNAME" => assigm_tk:"=" => value_tk:"VALUE" => sep_tk => name_tk:"VARNAME" => assigm_tk:"+" => value_tk:"VALUE"`

>`VARNAME = VALUE`  
>становится:  
>`expr_tk:"VARNAME" =>=> expr_tk:"=" =>=> expr_tk:"VALUE"`

# Math:

Математический оператор заключается в двойные круглые скобки `((MATH))` или объявляется с помощью `let "MATH"`.

>`((1 + 2 + 3))`  
>становится:  
>`math_tk:"1 + 2 + 3"`

>`let "1 + 2 + 3"`  
>становится:  
>`math_tk:"1 + 2 + 3"`

>`$((1 + 2 + 3))`  
>становится:  
>`deref_tk => math_tk:"1 + 2 + 3"`

# Subshell:

Исполнение команд в "подоболочке" происходит в отдельном процессе командной оболочке, все переменные объявленные внутри не переносятся в родительский процесс (нашу оболочку) и все изменения файл-дескрипторов и прочего остается в "подоболочке". Скорее всего будет реализована как обычный `execve()` или продвинутый `fork()`.

>`(cmd1 cmd1; cmd2 cmd2)`  
>становится:  
>`subsh_tk:"cmd1 cmd1; cmd2 cmd2"`

>`$(cmd1)`  
>становится:  
>`deref_tk => subsh_tk:"cmd1"`

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
>`if_tk =>=> expr_tk:"expr1" => sep_tk =>=> then_tk =>=> exprt_tk:"expr2" => sep_tk => .... => fi_tk`

*Новая линия может быть представлена отдельным токеном равно значным разделителю.*

# Functions:

Функции в баш похожи на выражения, так что их вызов ничем не будет отличаться от исполнения программы или встроенной функции в bash. Для объявления функции используется отдельный токен и отдельные инструкции интерпритатора.

Образец токенизации:


	function fname {cmd1; cmd2; cmd3}

>или

	fname() {cmd1; cmd2; cmd3}

>Реализуется как:  
>`function_tk =>=> name_tk:"fname: =>=> expr_tk:"cmd1" => sep_tk => expr_tk:"cmd2: => ... => fend_tk`

# Newline:

Токен новой линии по сути своей равен сепаратору команд.

# Apostroph and Double-quotes

Токены апострофа и двойных ковычек реализуют один expression в итоге. Одинарные ковычки экраннируют что угодно, а двойные не экранинуют смивол '$'  b '\'.

>`ls "a b c"`  
>становится:  
>`expr_tk:"ls" =>=> dquote_tk:"a b c" `

>**ВАРИАНТ ВРЕМЕННЫЙ**
