*This project has been created as part of the 42 curriculum by fkruger.*

# Pipex

## Description

a **pipex** is a program that replicates the behavior of Unix shell pipes (`|`).
It takes an input file, runs two commands, and writes the result to an output file—just like:

## Instructions

### building
```bash
make        # compile
make clean  # remove object files
make fclean # remove all binaries
make re     # rebuild
```

### Useage
```bash
./pipex infile "cmd1" "cmd2" outfile
```

should behave just like
```bash
< infile cmd1 | cmd2 > outfile
```
The supported of syntax of the cmds is kept minimalistic, the point of this program
is reproducing the execution part of bash pipes not the syntax of a command.

---

## Resources

* `man 2 pipe`
* `man 2 fork`
* `man 2 dup2`
* `man 3 execve`
* `man 7 errno.h`
* [bash manual: pipelines](https://www.gnu.org/software/bash/manual/html_node/Pipelines.html)
* my fellow students were the most valuable resources :)
---
