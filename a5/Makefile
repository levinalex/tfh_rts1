# RTS Aufgabe 1   -- 2008-07-21
# Levin Alexander -- 744463

CFLAGS = -Werror -Wall -std=c99
LDFLAGS = -pthread

test: bufcpy infile
	./bufcpy infile outfile
	diff infile outfile

infile:
	dd if=/dev/urandom of=infile count=1024 bs=1014

bufcpy: fileio.o

tar:
	tar -czf a5.tar.gz Makefile bufcpy.c fileio.c fileio.h

clean:
	rm infile
	rm outfile
	rm bufcpy
	rm -r *.o

all: bufcpy
	
