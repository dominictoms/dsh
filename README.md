# dsh

## Introduction
### What is dsh?
dsh is a basic shell for Unix and Unix-like operating systems written in C. The goal of dsh is to create a lightweight, portable and eventually POSIX compliant shell similar to Bash or Zsh. 
```
dsh 0.1$ echo "hello, world!"
hello, world!
```

### Uses for dsh
As of right now there is no real reason to use dsh. This project mainly exists for my own personal amusement as well as a tool to further my understanding of the underlying technology behind such applications. As of right now dsh is lacking in the features and stability required for any serious work to be done through the shell.

## Configuration
### Configuration File
Many global variables can be modified in the `config.h` header file, such as default parameters, file paths and environment variables.
```c
// default values
#define DEFAULT_PROMPT VERSION "$"
#define DEFAULT_HIST_LEN 100

// environment variable names
#define PROMPT_VAR "DSHPROMPT"
#define HIST_LEN_VAR "DSHHISTORYLEN"

// local files
#define HIST_FILE_NAME ".dshhistory"
#define TEMP_HIST_FILE_NAME ".tempdshhistory"
#define TEMP_FILE_DIR "/var/tmp"
```

### Environment Variables
dsh will read environment variables and use them as overrides to the default preferences set in the configuration file. For example, if you wanted to use a different prompt, you can simply set the `DSHHPROMPT` variable.
```
dsh 0.1$ DSHPROMPT="cool prompt$ "
cool prompt$ unset DSHPROMPT
dsh 0.1$
```

## Building
### Compiling dsh
dsh can be compiled through more or less any C compiler such as Clang or GCC.
```sh
dsh 0.1$ gcc dsh.c
```
