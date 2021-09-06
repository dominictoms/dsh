# dsh

## Introduction
### What is dsh?
dsh is a basic shell for Unix and Unix-like operating systems written in C. The goal of dsh is to create a lightweight, portable and eventually POSIX compliant shell similar to Bash or Zsh.
```sh
dsh 0.1$ echo hello, world!
hello, world!
```

### Uses for dsh
As of right now there is no real reason to use dsh. This project mainly exists for my own personal amusement as well as a tool to further my understanding of the underlying technology behind such applications. As of right now dsh is lacking in the features and stability required for any serious work to be done through the shell.

## Configuration
### Configuration File
Many global variables can be modified in the `config.h` header file, such as default parameters, file paths and environment variables.
```c
// default values
#define DEFAULTPROMPT "dsh " version "$ "
#define DEFAULTHISTLEN 100

// environment variables
#define PROMPTVAR "DSHPROMPT"
#define HISTLENVAR "DSHHISTORYLEN"

// local files
#define HISTFILENAME ".dshhistory"
#define TEMPHISTFILENAME ".tempdshhistory"
#define TEMPFILEDIR "/var/tmp"
```

### Environment Variables
dsh will read environment variables and use them as overrides to the default preferences set in the configuration file. For example, if you wanted to increase the amount of commands stored in the dsh history file, you can simply set the `DSHHISTORYLENGTH` variable.
```sh
$ export DSHHISTORYLENGTH=1000
```

## Building
### Compiling dsh
dsh can be compiled through more or less any C compiler such as Clang or GCC.
```sh
$ gcc dsh.c
```
