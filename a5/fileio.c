// RTS Aufgabe 1   -- 2008-06-23
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

ssize_t file_read(int fd, void* buf, size_t count) {
  ssize_t n = read(fd, buf, count);
  if (n < 0) { 
    perror("error while reading from source file");
    exit(3);
  }
  return n;
}

ssize_t file_write(int fd, const void *buf, size_t count) {
  ssize_t wcount = 0;

  while (wcount < count) {
    ssize_t result = write(fd, buf + wcount, count - wcount);
    if (result < 0) { exit(-1); }
    wcount += result;
  }
  return wcount;
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
    perror("error opening output file for writing");
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

