// RTS Aufgabe 1   -- 2008-06-09
// Levin Alexander -- 744463

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFSIZE (10)
#define BUCKETSIZE (1024)

int buffer[BUFSIZE][BUCKETSIZE];
size_t buffill[BUFSIZE];

sem_t buf_empty;
sem_t buf_full;

void reader(char* srcpath) {
  int srcfd = open(srcpath, O_RDONLY);
  int bufpos = 0;
  int rcount = 0;
  
  do {
    sem_wait(&buf_full); // wait until buffer is not full
    
    rcount = read(srcfd, buffer[bufpos], sizeof(buffer[bufpos]));

    if (rcount < 0) { 
      perror("Read error\n");
      exit(-1);
    }
    
    buffill[bufpos] = rcount;
    bufpos = (bufpos + 1) % BUFSIZE;
    sem_post(&buf_empty); // buffer is not empty
  } while(rcount > 0);
  
  close(srcfd);
}

void writer(char* destpath) {
  int destfd = open(destpath, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  int bufpos = 0;
  int wcount;
  int rcount;
  
  do {
    sem_wait(&buf_empty);
    
    wcount = 0;
    rcount = buffill[bufpos];
    
    while (wcount < rcount) {
      int result = write(destfd, buffer[bufpos] + wcount, rcount - wcount);
      if (result < 0) { exit(-1); }
      wcount += result;
    }
    
    bufpos = (bufpos + 1) % BUFSIZE;
    
    sem_post(&buf_full);
    
  } while(rcount > 0);
  
  close(destfd);
}

int main(int argc, char* argv[]) {
  pthread_t reader_thread;

  if (argc != 3) { 
    printf("USAGE: %s <source> <dest>\n", argv[0]); 
    exit(1); 
  }
  
  sem_init(&buf_empty, 0, 0);
  sem_init(&buf_full, 0, BUFSIZE);
  
  pthread_create(&reader_thread, 0, (void*)reader, (void*)argv[1]);
  
  writer(argv[2]);
  
  pthread_join(reader_thread, NULL);
  
  return 0;
}
