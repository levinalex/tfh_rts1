// RTS Aufgabe 1   -- 2008-04-20
// Levin Alexander -- 744463

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
	int pid;
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

int main(int argc, char* argv[]) {

	exit(0);
} 
