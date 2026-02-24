#!/bin/sh

valgrind --trace-children=yes --track-fds=all ./pipex "$1" "$2" "$3" "$4"
