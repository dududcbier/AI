/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAXQ 100

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */
int solved = 0;

typedef struct board {

  int queens[11];
  int fitness;

} Board;


void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
  // time_t t;
  // srand((unsigned) time(&t));
  srandom((unsigned int)time(NULL));
  srand((unsigned int)time(NULL));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0? 0 : random()%nqueens); 
  }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

int countConflictsOnBoard(int *x) {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, x[queen], other, x[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

int evaluateBoard(int *x) {
  return (nqueens-1)*nqueens/2 - countConflictsOnBoard(x);
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = random() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
    solved++;
  }
  printf ("Final state is");
  printState();
}

/*************************************************************/

void calculateNeighborsHC(int *neighbors, int queen){
  for (int pos = 0; pos < nqueens; pos++){
    moveQueen(queen, pos);
    neighbors[pos] = evaluateState();
  }
}

int bestNeighbor(int queen){
  int neighbors[MAXQ];
  int best = -1;
  int sameValue[MAXQ], sizeSameValue = 0;
  calculateNeighborsHC(neighbors, queen);
  for (int i = 0; i < nqueens; i++){
    if (neighbors[i] > best){
      best = neighbors[i];
      sizeSameValue = 0;
    }
    if (neighbors[i] == best){
      sameValue[sizeSameValue] = i;
      sizeSameValue++;
    }
  }

  return sameValue[random() % sizeSameValue];
}

void hillClimbing(int limit) {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;
  int eval;

    /* generate new, better state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int newpos;
      /* change to one of the best neighbors */
      newpos = bestNeighbor(queen);
      moveQueen(queen, newpos);
    }

  eval = evaluateState();
  if (eval == optimum) {
    printf ("Solved puzzle. ");
    solved++;
  }
  else
    printf ("Puzzle not solved. Final state is");

  printState();

  // Try again using the final state as initial state?
  if (eval != optimum && limit > 0){
    initiateQueens(1);
    hillClimbing(limit - 1);
  }

}

/*************************************************************/

float T(int t){
  return 100 * pow(0.95, t);
}

double P(int deltaE, float temperature){
  return exp(deltaE / temperature) * ((double) RAND_MAX + 1.0);
}

void simulatedAnnealing() {
  unsigned int t;
  float temperature;
  int nextQueen, nextPos, oldPos;
  int current, deltaE, eval;
  int optimum = (nqueens-1)*nqueens/2;

  for (t = 0; TRUE; t++){
    temperature = T(t);

    if (temperature <= 0)
      break;

    nextQueen = random() % nqueens;
    nextPos = random() % nqueens;

    oldPos = queens[nextQueen];
    while (oldPos == nextPos)
      nextPos = random() % nqueens;

    current = evaluateState();
    moveQueen(nextQueen, nextPos);
    deltaE = evaluateState() - current;

    // Undo the move if it's not a good move, but only with probability 1 - e^(deltaE / temperature)
    if (deltaE <= 0 && rand() > P(deltaE, temperature)){
      moveQueen(nextQueen, oldPos);
    }
  }

  eval = evaluateState();
  if (eval == optimum) {
    printf ("Solved puzzle. ");
    solved++;
  }
  else
    printf ("Puzzle not solved. Final state is");

  printState();

}

/*************************************************************/

void reproduce(int *x, int *y, Board* child) {
  int c = 1 + rand() % (nqueens - 1);
  for (int i = 0; i < c; i++)
    child->queens[i] = x[i];

  for (int i = c; i < nqueens; i++)
    child->queens[i] = y[i];

  child->fitness = evaluateBoard(child->queens);
}

void swapPermutation(int *x, int left, int right){
  int aux;
  aux = x[left];
  x[left] = x[right];
  x[right] = aux;
}

void shiftPermutation(int *x, int left, int right){
  for (int i = right; i > left + 1; i--)
    swapPermutation(x, i - 1, i);
}

void reversalMutation(int *x, int left, int right){
  while (left < right)
    swapPermutation(x, left++, right--);
}

void scrambleMutation(int *x, int left, int right){
  if (right != left)
    for (int i = left; i < right + 1; i++){
      int r = left + rand() % (right - left);
      swapPermutation(x, i, r);
    }
}

void mutate(int *x){
  int left = rand() % nqueens;
  int right = rand() % nqueens;

  if (left > right){
    int aux = right;
    right = left;
    left = aux;
  }

  switch(rand() % 4){
    case 0: shiftPermutation(x, left, right); break;
    case 1: swapPermutation(x, left, right);  break;
    case 2: reversalMutation(x, left, right); break;
    case 3: scrambleMutation(x, left, right); break;
  }
}

void populate(Board *pop) {
  int q;
  for (q = 0; q < nqueens; q++)
    pop->queens[q] = rand()%nqueens; 

  pop->fitness = evaluateBoard(pop->queens);
}

int binarySearch(int *a, int size, int value){
  int low = 0;
  int high = size - 1;

  while (low <= high){
    int mid = (low + high) / 2;

    if (a[mid] >= value)
      high = mid - 1;

    else
      low = mid + 1;
  }
  return low;
}

int binarySearchBoard(Board *b, int size, int value){
  int low = 0;
  int high = size - 1;

  while (low <= high){
    int mid = (low + high) / 2;

    if (b[mid].fitness <= value)
      high = mid - 1;

    else
      low = mid + 1;
  }
  return low;
}

void insertSorted(Board *population, int size, Board new){
  // Position for board to be inserted
  int pos = binarySearchBoard(population, size, new.fitness);

  population[size - 1] = new;

  // Shifting to the right position
  for (int i = size - 1; i > pos; i--){
    Board aux = population[i - 1];
    population[i - 1] = population[i];
    population[i] = aux;
  }

}

// Returns the position of a certain (random) board. Boards with higher values
// have better odds to be selected.
int select(Board *population, int size, int ignore){

  int accumulatedFitness[size];
  int r;

  accumulatedFitness[0] = population[0].fitness;
  for (int i = 1; i < size; i++)
    if (i != ignore)
      accumulatedFitness[i] = population[i].fitness + accumulatedFitness[i - 1];
    else if (ignore != -1)
      accumulatedFitness[i] = accumulatedFitness[i - 1];

  r = rand() % (accumulatedFitness[size - 1] + 1);

  return binarySearch(accumulatedFitness, size, r);
}

int cmpfunc(const void * a, const void * b){
   const void *left = (const void **) a;
   const void *right = (const void **) b;
   const Board *board1 = left;
   const Board *board2 = right;
   return ( (*board2).fitness - (*board1).fitness);
}

void geneticAlgorithm(){

  int size = 10;
  Board population[size];
  int optimum = (nqueens-1)*nqueens/2;
  int iter = 0;
  int chance = 1;

  for (int i = 0; i < size; i++)
    populate(&population[i]);

  while (evaluateState() != optimum && iter < 1000){
    for (int i = 0; i < size; i++){
      Board child;
      int index = select(population, size, -1);
      Board x = population[index];
      Board y = population[select(population, size, index)];

      reproduce(x.queens, y.queens, &child);

      if (rand() % 100 <= chance)
        mutate(child.queens);

      population[size-1] = child;
      insertSorted(population, size, child);
    }
    iter++;
  }  

  if (iter < 1000)
    solved++;
}

/*************************************************************/

void printBoard(Board *b){
  printf("Address: %p\nBoard: ", b);
  for (int i = 0; i < nqueens; i++)
    printf("%d ", b->queens[i]);
  printf("\nFitness: %d\n\n", b->fitness);
}

void checkSorting(Board *b, int size){
  for(int i = 0; i < size - 1; i++)
    assert(b[i].fitness >= b[i + 1].fitness);
}

int main(int argc, char *argv[]) {
  int algorithm;
  int repeat = 1;
  int v[11] = {5, 10, 20, 20, 46, 48, 67, 79, 93, 96, 100};
  int r;
  initializeRandomGenerator();

  // Board x, y, z;
  // Board boards[10];
  // populate(&x);
  // populate(&y);
  // nqueens = 11;
  // qsort(boards, 10, sizeof(Board), cmpfunc);
  // checkSorting(boards, 10);

  // insertSorted(boards, 10, x);
  // checkSorting(boards, 10);

  // insertSorted(boards, 10, y);
  // checkSorting(boards, 10);

  // printf("Sorting working\n");

  if (argc == 2)
    repeat = atoi(argv[1]);

  do {
    printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
    printf ("(3) Simulated Annealing  (4) Genetic Algorithm: ");
    scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));
  
  initializeRandomGenerator();

  for (int i = 0; i < repeat; i++){
    initiateQueens(1);
    
    printf("\nInitial state:");
    printState();

    switch (algorithm) {
    case 1: randomSearch();       break;
    case 2: hillClimbing(5);       break;
    case 3: simulatedAnnealing(); break;
    case 4: geneticAlgorithm(); break;
    }
  }
  printf("Solved %.1f%%\n", ((float) solved / repeat) * 100);

  return 0;  
}