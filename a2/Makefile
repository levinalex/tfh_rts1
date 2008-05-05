# RTS Aufgabe 1   -- 2008-04-20
# Levin Alexander -- 744463

CFLAGS = -Werror -Wall -std=c99
LDFLAGS = -pthread

all: empty main
	./main 10

stat: all
	time ./main 1000 | ./statistics

empty: empty.c
main: main.c

.PHONY: all stat
