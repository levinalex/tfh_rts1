// RTS Aufgabe 1   -- 2008-05-04
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

// the nothing, which gets done
void threadfunc(int* args) {}

// spawns a new pthread (which does nothing) and waits for it to terminate
//
void spawn_thread() {
	pthread_t tid;
	long status;
	
	if (pthread_create(&tid, NULL, (void *)threadfunc, NULL) == -1) {
		perror("create");
		exit(-1);
	}
	
	if (pthread_join(tid, (void **) &status) == -1) {
		perror("join");
		exit(-1);
	}
	
	return;	
}

// spawns a new process (which does nothing) and waits for it to terminate
//
void spawn_process() {
	pid_t pid;
	int stat;
	
	if ((pid = fork()) < 0) {
		perror("fork"); 
		exit(-1);
	} else {
		if (pid > 0) {
			waitpid(pid, &stat, 0);
		} else { 
			threadfunc(NULL);
			exit(0);
		}
	}
}

long measure(void(blk)()) {
	struct timeval start, end;
	long diff = 0;
	
	// measure
	gettimeofday(&start,0);
	blk();
	gettimeofday(&end,0);
	
	// calculate duration in usec
	diff = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	return(diff);
}

int main(int argc, char* argv[]) {

	printf("%ld\n", measure(spawn_process));
	exit(0);
} 
