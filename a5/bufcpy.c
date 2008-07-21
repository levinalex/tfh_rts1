// RTS Aufgabe 1   -- 2008-07-21
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "fileio.h"

#define BUFSIZE (735)
#define BUCKETSIZE (397)

int buffer[BUFSIZE][BUCKETSIZE];
size_t buffill[BUFSIZE];

sem_t buf_empty;
sem_t buf_full;

// read data from source file into ringbuffer
//
void reader(char* srcpath) {
  int srcfd = file_open_read(srcpath); // open source file for reading
  int bufpos = 0; // next free space in ringbuffer
  int rcount;     // number of bytes read on last iteration
  
  do {
    sem_wait(&buf_full); // wait until buffer is not full
    
    // read from sourcefile into buffer
    rcount = file_read(srcfd, buffer[bufpos], sizeof(buffer[bufpos]));

    // store number of bytes read and advance write head
    buffill[bufpos] = rcount;
    bufpos = (bufpos + 1) % BUFSIZE;

    sem_post(&buf_empty); // increase number of cells written to buffer (unblock writer)

  } while(rcount > 0); // abort on EOF (zero bytes read from source)
  
  file_close(srcfd);
}

// write date from ringbuffer into destination file
//
void writer(char* destpath) {
  int destfd = file_open_write(destpath); // open destination file for writing
  int bufpos = 0; // next unread space in ringbuffer
  int wcount;     // number of bytes written on last iteration
  
  do {
    sem_wait(&buf_empty); // wait until buffer is not empty
    
    // write contents of buffer into destination file
    wcount = file_write(destfd, buffer[bufpos], buffill[bufpos]);
    // advance read head
    bufpos = (bufpos + 1) % BUFSIZE;
    
    sem_post(&buf_full); // mark buffer as not full
    
  } while(wcount > 0); // abort on EOF (zero bytes in current buffer cell)
  
  file_close(destfd);
}

int main(int argc, char* argv[]) {
  if (argc != 3) { 
    printf("USAGE: %s <source> <dest>\n", argv[0]); 
    exit(1); 
  }
  
  sem_init(&buf_empty, 0, 0);
  sem_init(&buf_full, 0, BUFSIZE);
  
  pthread_t reader_thread;
  pthread_t writer_thread;

  pthread_create(&reader_thread, 0, (void*)reader, (void*)argv[1]);
  pthread_create(&writer_thread, 0, (void*)writer, (void*)argv[2]);
  
  pthread_join(reader_thread, NULL);
  pthread_join(writer_thread, NULL);
  
  return 0;
}
