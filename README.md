# 42sh

Ecole 42 project. UNIX shell implementation from scratch.

## Little foreword

Project writed in accordance with the [Norm](https://cdn.intra.42.fr/pdf/pdf/960/norme.en.pdf),
so if you're shocked by the way the code is written, it's her fault. Most of standart library functions
like `strlen`, `printf`, `memcpy` and some data structures (hash-table, binary tree, linked lists and strings)
are implemented in other project named [Libft](https://github.com/hgranule/Libft). All what we allowed to use for this project
is system-calls, or some basic functions like `malloc`, `free`, etc.

## Compilation

```
# clone it and use `make`

$> git clone
$> make
$> ./42sh
```

Some errors and warnings could be ocured at the compilation on Linux with `gcc`.
Project have been written at macOS Sierra with `clang`.

## Some screenshots

Here is some screentshots of our shell's work.

![Image example of commands](https://i.ibb.co/mDHWNsY/Screen-Shot-2020-01-09-at-16-53-10.png "Some cases")

![Image example with shell scripts](https://i.ibb.co/bKxh71j/Screen-Shot-2020-01-09-at-16-56-41.png "Some scripts")

![Image example with jobs](https://i.ibb.co/gVLQ9GY/Screen-Shot-2020-01-09-at-16-58-56.png "Some jobs")

## Objectives from subject

As a coder in training, there are moments, which marks your life. Forever. 42sh is one
of those moments. Achieving this project is a milestone at 42.

This project is about writing the most stable and most complete UNIX shell possible.
You already know a lot of shells and each has its own features, from the humble sh available on every UNIX distribution in the world to the complete and complex zsh, which
many of you use without really knowing why. There are many other shells, such as bash,
csh, tcsh, ksh, ash, etc. 42sh is your first real shell. It is common practice for students
to choose to use a reference shell and try to replicate its basic behavior. This can be a
good strategy if you know the reasons why you chose that one shell specifically. Consider
this. If you choose zsh as reference, you will need to commit to a long and difficult quest,
even if highly instructive. Through this quest, you will learn humility and what it means
to work hard.

So what to do? We suggest you try a couple of shell first to get an idea of their suble
and twisted differences. However, don’t be fooled by the apparent simplicity of the sh
shell. Recoding a thorough and stable sh is an achievement in itself. Forget about the
“moons and stars” shells. At the end, they won’t be able to do more than just a few
pipes and redirections.

The keyword here is “stability”. A humble and indestructible 42sh is always better
than a kooky 42sh with countless features that segfaults in unanticipated ways. The
latter will be worth 0 in the end. Make sure that the 42sh you submit is stable. I cannot
insist enough on that last point.

## Prerequests from subject

Main part of shell:
- Launching of bin ary executable files (+ PATH variable managment)
- Builtins: `echo`, `exit`, `cd` (+ CDPATH), `export`, `env`, `unset`, `type`
- Redirection and pipes: `>` `>>` `<<` `<&` `>&` `|`
- Logical operators and separators: `&&` `||` `;`
- Backround lauchning: `&`
- Enviroment and local variables
- Signals monitoring
- Job control with it's builtins `jobs`, `fg`, `bg`
- Command line editing and history of commands

Bonuses:
- Inhibitors `"`, `'`, `\`
- Pattern matching `[a-Z]!?*`
- Control groups and sub-shells `()`, `{}`
- Control substitution `$()`
- Ariphmetic operator with substitution `(())`, `$(())`
- Process substitution `<()`, `>()`
- Dinamic autocompletion using TAB: variables, commands and files
- Caching and `hash` builtin
- `test` builtin

Extra bonuses:
- Shell scripting: `if else`, `for in`, `for (())`, `while`, `function`
- File input
- Propmpt coloration and modificators (domain name, time, last status and etc)
- Caching of executables to a absolute path
- Binary tree for PATH management

Authors:
@Incomprehensible
@gdaemoni
@zidanez
@hgranule
