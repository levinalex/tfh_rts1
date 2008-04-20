#define FELDMAX 20
 
#include <stdio.h>

void sortiere(int field[], int count);
void mische(int in[], int out[], int count); 
 
void sortiere(int feld[], int anz) 
{ 
	int i = 0;
	int j = 0;
	for (i=0; i<anz; i++) {
		for (j = i; j<anz; j++) {
			if (feld[i] > feld[j]) { 
				int tmp = feld[i]; 
				feld[i] = feld[j]; 
				feld[j] = tmp;
			}
		}
	}
} 

// merge two sorted halves of the list +in+ into the
// output +out+
//
void mische(int in[], int out[], int count) { 
	int center = count/2;
	
	int i = 0;          // index into the first halve
	int j = 0 + center; // index into the second halve
	int k = 0;          // index into the result
	
	// until one of both input halves is exhausted, repeatedly copy the
	// smaller element from the heads into the result list.
	while (i < center && j < count) {
		if (in[i] < in[j]) { 
			out[k] = in[i]; i++;
		} else {
			out[k] = in[j]; j++;
		}
		k++;
	}
	// copy the remaining elements from the input list that was not 
	// yet exhausted
	while (i < center) { out[k] = in[i]; i++; k++; }
	while (j < count)  { out[k] = in[j]; j++; k++; }
	
} 
 
int main(int argc, char* argv[]) {
	int feld[FELDMAX];
	int feldaus[FELDMAX]; 
	
	// Füllen von feld mit Zufallszahlen mit Werten von 0 bis 999 
	int i;
	int rnd = open("/dev/random", "r");
	for (i=0; i<FELDMAX; i++) {
		feld[i] = rand() % 1000;
		printf("%i\n", feld[i]);
	}
	
	
	sortiere(feld, FELDMAX/2); 
	sortiere(feld + FELDMAX/2, FELDMAX/2); 
	mische(feld, feldaus, FELDMAX);

	printf("----------\n");
	
	for (i=0; i<FELDMAX; i++) {
		printf("%i\n", feldaus[i]);
	}
	
	exit(0);
} 
