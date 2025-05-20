# 🐚 minishell

> A simplified shell that mimics bash behavior. Built for the 1337 School curriculum.

## 📌 Description

The **minishell** project is about creating a simple Unix shell that can interpret and execute user commands. It supports features like pipes, redirections, environment variables, built-ins, and signal handling — all without using `system()` or `execvp()`.

## 🎯 Objectives

- Parse and tokenize complex input strings.
- Implement redirections: `>`, `>>`, `<`, `<<` (heredoc).
- Handle pipelines: `ls | grep txt | wc -l`.
- Execute built-in commands: `cd`, `echo`, `env`, `export`, `unset`, `pwd`, `exit`.
- Manage environment variables.
- Handle signals like `Ctrl+C`, `Ctrl+D`.
- Ensure memory is properly freed (no leaks).
- Follow 1337 Norm and allowed functions only.

## ⚙️ Allowed Functions

```c
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay,
add_history, printf, malloc, free, write, access, open, read, close, fork,
wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill,
exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe,
opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
```

## 📁 Project Structure

```
minishell/
├── src/
│   ├── parser/
│   ├── executor/
│   ├── builtins/
│   ├── signals/
│   └── ...
├── include/
│   └── minishell.h
├── libft/
├── Makefile
└── README.md
```

## ▶️ Example

```bash
$ ./minishell
minishell$ echo hello world
hello world
minishell$ export NAME=OB4413
minishell$ echo $NAME
OB4413
minishell$ ls | grep .c | wc -l
```

## 🛠️ Build

```bash
make
./minishell
```

## ✅ Features

- Custom command-line interface
- Pipes and redirections
- Environment variable management
- Built-in commands
- Signal handling (SIGINT, SIGQUIT)
- Clean memory management

## ❌ Limitations

- No wildcard expansion (`*`, `?`)
- No logical operators (`&&`, `||`)

## 👨‍💻 Author

- GitHub: [OB4413](https://github.com/OB4413)
