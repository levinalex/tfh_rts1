// RTS Aufgabe 1   -- 2008-06-09
// Levin Alexander -- 744463

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N (20)
#define TIME_UNIT (0)

sem_t guard_print;
sem_t guard_fork[N];
sem_t guard_deadlock;

int stats[N];
pthread_t dining_philosopher[N];
int want_exit = 0;

void output(int i, char* text) {
	sem_wait(&(guard_print));
	printf("%2d %s\n", i, text);
	sem_post(&(guard_print));
}

void idle(int time) {
	usleep(time * TIME_UNIT);
}

void eat(int i) {
	sem_wait(&guard_deadlock);      // check if eating is theoretically possible
	sem_wait(&guard_fork[i]);       // take left fork
	sem_wait(&guard_fork[(i+1)%N]); // take right fork

	output(i, "eating");
	idle(2);

	stats[i] += 1; // count eating // count eating
	
	sem_post(&guard_fork[i]);       // put down left fork
	sem_post(&guard_fork[(i+1)%N]); // put down right fork
	sem_post(&guard_deadlock);
}

void think(int i) {
	output(i, "thinking");
	idle(3);
}

void dine(void* i) {
	int number = (int)i;
	while (1) {
		think(number);
		eat(number);
		if (want_exit) { pthread_exit(0); }
	}
	return;
}

void print_statistics() {
	float max = 0;
	int c = 0;
  int i = 0;
  
  printf("\nStatistics\n==========\n");
  
  // find max, to scale bar graphs
	for (i=0; i<N; i++) { if (max < stats[i]) { max = stats[i]; } }

	for (i=0; i<N; i++) { 
		printf("%2d: %5d ", i, stats[i]);
		for (c=0; c< 100 * stats[i]/max ; c++) { printf("+"); }
		printf("\n");
	}
}

void at_exit(void) {
	want_exit = 1; 	// tell all threads to quit
}

int main(int argc, char** argv) {
	signal(SIGINT, (void*)at_exit);

	int i = 0;
  
	sem_init(&guard_print, 0, 1);
	sem_init(&guard_deadlock, 0, N-1);
	
	for (i=0; i<N; i++) { sem_init(&guard_fork[i], 0, 1); }
	for (i=0; i<N; i++) { stats[i] = 0; }
	
	// start up philosophers
	for (i=0; i<N; i++) {
		pthread_create(&dining_philosopher[i], 0, (void*)dine, (void*)i);
	}

  // wait for all threads to exit
  for (i=0; i<N; i++) { pthread_join(dining_philosopher[i], NULL); }

  print_statistics();
  exit(0);
}


