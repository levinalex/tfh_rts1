// RTS Aufgabe 4   -- 2008-06-23
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "fileio.h"

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {
  
  // check for correct number of arguments
  //
  if (!argc == 3) { 
    printf("USAGE: %s <source> <dest>\n", argv[0]); 
    exit(1); 
  }

  // open source and destination files
  int sourcefile = file_open_read(argv[1]);
  int destfile = file_open_write(argv[2]);
  
	int pid;
 	int stat;

	int pipefd[2];
	pipe(pipefd);

	if ((pid = fork()) < 0) {
		perror("fork"); 
		exit(0);
	} else {
		if (pid > 0) {		  
		  // close unneeded files
			file_close(pipefd[READ]);
      file_close(destfile);
      
      // write from source into pipe
			filecpy(sourcefile, pipefd[WRITE]);
      
      // close rest of files
      file_close(sourcefile);
			file_close(pipefd[WRITE]);
			
			// wait for child to exit
			waitpid(pid, &stat, 0);
		} else {
		  // close unneeded files
			file_close(pipefd[WRITE]);
			file_close(sourcefile);
			
			// write from pipe into destination
			filecpy(pipefd[READ], destfile);
      
      // close rest of files
      file_close(destfile);
			file_close(pipefd[READ]);
			
			exit(0);
		}
	}
	  
	exit(0);
}