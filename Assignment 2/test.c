#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main (){

	for (int i = 0; i < 10; i++)
		printf("%ld ", random() % 10);
	printf("\n");

	srand(time(NULL));

	for (int i = 0; i < 10; i++)
		printf("%ld ", random() % 10);
	printf("\n");	
	
	for (int i = 0; i < 10; i++)
		printf("%f ", exp(i));
	printf("\n");

	printf("RAND MAX: %d\n", RAND_MAX);
	for(int i = 0; i < 10; i++)
		printf("%d ", rand());
	printf("\n");

	return 0;
}