// RTS Aufgabe 1   -- 2008-06-23
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLEN (100000)

// copy data from a source file to a destination
//
//   +src+: file descriptor, must be open for reading
//   +dst+: file descriptor, must be open for writing
//
// returns zero on success, and a negative 
// value on failure
//
int filecpy(int src, int dst) {
  char buf[MAXLEN];
  
  size_t rcount = 0;
  size_t wcount = 0;
  
  while ((rcount = read(src, buf, sizeof(buf))) > 0) {
    wcount = 0;
    while (wcount < rcount) {
      int result = write(dst, buf + wcount, rcount - wcount);
      if (result < 0) { return(-1); }
      wcount += result;
    }
  }
  return rcount; 
}


int file_open_read(char* path) {
  int fd = open(path, O_RDONLY);
  if (fd <= 0) {
    perror("error opening input file for reading");
    exit(2);
  }
  return fd;
}

int file_open_write(char* path) {
  int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  if (fd <= 0) {
    perror("error opening input file for reading");
    exit(2);
  }
  return fd;
}

void file_close(int fd) {
  int result = close(fd);
  if (result < 0) {
    perror("error closing file");
    exit(2);
  }
}