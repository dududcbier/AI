#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEPTH 100000
#define RANGE 1000000
#define UNDISCOVERED 0
#define DISCOVERED 1

typedef enum {ADD, TRIPLE,  DOUBLE, SUBTRACT, THIRD, HALF} operation;

int status[3*RANGE] = {0};
int verbose = 0;
int solution[RANGE];

int min(int x, int y){
	if (x < y) return x;
	return y;
}

int max(int x, int y){
	if (x > y) return x;
	return y;
}

/* Calculates the neighbors of x, only considers neighbors that move towards the goal */
void succ(int x, int *successors, int goal){
	if (x <= goal){
		successors[ADD] = x + 1;
		if (x != 0){
			successors[DOUBLE] = x * 2;
			successors[TRIPLE] = x * 3;
		}
		else {
			successors[DOUBLE] = successors[TRIPLE] = -1;
		}
		successors[THIRD] = -1;
		successors[HALF] = -1;
		successors[SUBTRACT] = -1;
	}
	else {
		successors[ADD] = -1;
		successors[DOUBLE] = -1;
		successors[TRIPLE] = -1;
		successors[SUBTRACT] = x - 1;
		if (x != 0){
			successors[HALF] = x / 2;
			successors[THIRD] = x / 3;
		}
		else {
			successors[HALF] = successors[TRIPLE] = -1;
		}
	}
}

int dfsLimited(int start, int goal, int maxDepth, int currentDepth, int *nodeCount){
	int result, next;
	int successors[6] = {-1};

	status[start] = DISCOVERED;
	*nodeCount += 1;
	if (verbose)
		printf("%d discovered\n", start);

	if (start == goal) {
		if (verbose)
			printf("Goal reached!\n");
		return currentDepth;
	}

	if (currentDepth > maxDepth){
		if (verbose)
			printf("Maximum depth reached (maxDepth = %d)\n", maxDepth);
		return -1;
	}

	/* Calculating neighbors */
	succ(start, successors, goal);

	/* Visiting neighbors */
	for (int i = 0; i < 6; i++){
		next = successors[i];
		result = -1;
		if (next >= 0){
			result = dfsLimited(next, goal, maxDepth, currentDepth + 1, nodeCount);
			if (result != -1){
				if (verbose)
					printf("Found it!\n");
				solution[currentDepth + 1] = i;
				return result;
			}
		}
	}

	if (verbose)
		printf("%d visited\n", start);

	return result;
}

int ids(int start, int goal, int *nodeCount){

	int result = -1;

	for (int depth = 0; depth < MAX_DEPTH; depth++){
		/* Reseting visited nodes */
		for (int i = min(start, goal) / 3; i < max(start, goal) * 3; i++)
			status[i] = UNDISCOVERED;

		result = dfsLimited(start, goal, depth, 0, nodeCount);
		if(verbose)
			printf("\n");
		if (result != -1)
			return result;
	}

	return result;

}

int costOf(operation op) {
  int c;
  switch(op){
    case ADD:
    case SUBTRACT:
      c = 1;
      break;
    case THIRD:
    case HALF:
      c = 3;
      break;
    case DOUBLE:
    case TRIPLE:
      c = 2;
      break;
    default:
      c = 0;
      break;
  }
  return c;
}

void printSolution(int start, int d, int nodeCount){
	int x, cost = 0;

	printf("%d ", start);
	x = start;
	for (int i = 1; i <= d; i++){
		cost += costOf(solution[i]);
		switch(solution[i]){
			case ADD:
				printf("(+1)-> %d ", ++x);
				break;
			case SUBTRACT:
				printf("(-1)-> %d ", --x);
				break;
				case HALF:
				x /= 2;
				printf("(/2)-> %d ", x);
				break;
			case DOUBLE:
				x *= 2;
				printf("(*2)-> %d ", x);
				break;
			case THIRD:
				x /= 3;
				printf("(/3)-> %d ", x);
				break;
			case TRIPLE:
				x *= 3;
				printf("(*3)-> %d ", x);
				break;
			default:
				printf("ERROR ");
				break;
		}

	}
	printf("\nlength: %d, cost %d",d, cost);
	printf("\n\n");
	printf("Goal reached! (%d nodes visited)\n", nodeCount);

}

int main(int argc, char *argv[]){

	int start, goal, d, nodeCount = 0;
	
	if (argc < 3) {
		fprintf(stderr, "Usage: %s start goal [-verbose/-v]\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (argc == 4 && (strcmp(argv[3], "-v") == 0 || strcmp(argv[3], "-verbose") == 0)){
		verbose = 1;
	}

	start = atoi(argv[1]);
	goal = atoi(argv[2]);

	printf("Problem: route from %d to %d\n\n", start, goal);

	d = ids(start, goal, &nodeCount);

	if (d >= 0){
		printSolution(start, d, nodeCount);
	}

	else {
		printf("Solution not found.\n");
	}

	return 0;
}