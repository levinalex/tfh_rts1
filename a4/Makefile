# RTS Aufgabe 1   -- 2008-06-22
# Levin Alexander -- 744463

CFLAGS = -Werror -Wall -std=c99

test: pipecpy
	./pipecpy infile outfile
	diff infile outfile

pipecpy: fileio.o

all: pipecpy

clean:
	rm -f outfile
	rm -f *.o
	rm -f pipecpy

.PHONY: all
