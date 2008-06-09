#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N (3)

sem_t guard_print;
sem_t guard_fork[N];
sem_t guard_deadlock;

void output(int i, char* text) {
	sem_wait(&(guard_print));
	printf("%d %s\n", i, text);
	sem_post(&(guard_print));
}

void eat(int i) {
  sem_wait(&guard_deadlock);      // check if eating is theoretically possible
  output(i, "   trying to eat");
	sem_wait(&guard_fork[i]);       // take left fork
	output(i, "   taken left fork");
  sleep(1);
	sem_wait(&guard_fork[(i+1)%N]); // take right fork
	output(i, "   taken right fork");
	output(i, "eating");
	sleep(1);
	
	sem_post(&guard_fork[i]);       // put down left fork
	sem_post(&guard_fork[(i+1)%N]); // put down right fork
  sem_post(&guard_deadlock);
}

void think(int i) {
	output(i, "thinking");
	sleep(3);
}

void dine(void* i) {
	int number = (int)i;
	while (1) {
		think(number);
		eat(number);
	}
	return;
}

int main(int argc, char** argv) {
  pthread_t threads[N];
  int i = 0;
  
	sem_init(&guard_print, 0, 1);
  sem_init(&guard_deadlock, 0, N-1);
  for (i=0; i<N; i++) { sem_init(&guard_fork[i], 0, 1); }
	
	// start up philosophers
	for (i=0; i<N; i++) {
		pthread_create(&threads[i], 0, (void*)dine, (void*)i);
	}

  // wait for all threads to quit
  for (i=0; i<N; i++) { pthread_join(threads[i], NULL); }
}