#define FELDMAX 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sort(int field[], int nelem);
void merge(int in[], int count); 

void forking_mergesort(int field[], int nelem) {
	int fd[2];
	int pid;
	int stat;
	
	if (nelem <= 1) { return; } // termination condition

	pipe(fd);

	if ((pid = fork()) < 0) {
		perror("fork"); 
		exit(0);
	} else {
		if (pid > 0) {
			// in parent process
			close(fd[1]);
			
			// sort the first half and wait for termination of child
			sort(field, nelem/2);
			waitpid(pid, &stat, 0);
			
			// read data from child
			read(fd[0], field+nelem/2, nelem/2 * sizeof(int));
			close(fd[0]);
		} else { 
			// in child process
			close(fd[0]);

			// sort second half
			sort(field+nelem/2, nelem - nelem/2);
			write(fd[1], field+nelem/2, nelem/2 * sizeof(int));
			
			close(fd[1]);
			exit(0);
		}
		merge(field, nelem);
	}
}

// very simple insertion sort (not used)
//
void sort(int field[], int nelem) { 
	int i = 0;
	int j = 0;
	for (i=0; i<nelem; i++) {
		for (j = i; j<nelem; j++) {
			if (field[i] > field[j]) { 
				int tmp = field[i]; 
				field[i] = field[j]; 
				field[j] = tmp;
			}
		}
	}
}

// merge two sorted halves of the list +in+ into
//
void merge(int in[], int count) {
	int tmp[count];
	int center = count/2;
	
	int i = 0;          // index into the first half
	int j = 0 + center; // index into the second half
	int k = 0;          // index into the result
	
	// until one of both input halves is exhausted, repeatedly copy the
	// smaller element from the heads into the result list.
	while (i < center && j < count) {
		if (in[i] < in[j]) { 
			tmp[k] = in[i]; i++;
		} else {
			tmp[k] = in[j]; j++;
		}
		k++;
	}
	// copy the remaining elements from the input list that was not 
	// yet exhausted
	while (i < center) { tmp[k] = in[i]; i++; k++; }
	while (j < count)  { tmp[k] = in[j]; j++; k++; }
	
	memcpy(in, tmp, sizeof(int)*count);	
}
 
int main(int argc, char* argv[]) {
	int field[FELDMAX];
	srand(time());
	
	// fill field with random values 0...1000
	int i;
	for (i=0; i<FELDMAX; i++) {
		field[i] = rand() % 1000;
		printf("%i ", field[i]);
	}
	
	printf("\n---\n");
	
	forking_mergesort(field, FELDMAX);
	
	// print the sorted field
	for (i=0; i<FELDMAX; i++) { printf("%i ", field[i]); }
	printf("\n");
	
	exit(0);
} 
