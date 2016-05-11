#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"

#define RANGE 1000000
typedef enum {NUL, ADD, DOUBLE, TRIPLE, SUBTRACT, HALF, THIRD} operation;

void printRoute(int target, int start);
int costOf(operation op);

int visited[RANGE];
operation operationToGetTo[RANGE] = {NUL};

Fringe insertValidSucc(Fringe fringe, int value, int parent, int op, int prevCost) {
  State s;
  if ((value < 0) || (value > RANGE) || visited[value] != -1) {
    /* ignore states that are out of bounds or that have already been reached */
    return fringe;
  }

  s.value = value;
  visited[value] = parent;
  operationToGetTo[value] = op;
  return insertFringe(fringe, s, costOf(op) + prevCost);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value, cost;

  fringe = makeFringe(mode);
  state.value = start;
  fringe = insertFringe(fringe, state, 0);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    cost = state.cost;
    if (value == goal) {
      goalReached = 1;
      printRoute(value, start);
      break;
    }
    /* insert neighbouring states */
    if (value > goal){
      fringe = insertValidSucc(fringe, value-1, value, SUBTRACT, cost); /* rule n->n - 1      */
      if (value != 0){
        fringe = insertValidSucc(fringe, value/2, value, HALF, cost); /* rule n->floor(n/2) */
        fringe = insertValidSucc(fringe, value/3, value, THIRD, cost); /* rule n->floor(n/3) */
      }
    }
    else {
      fringe = insertValidSucc(fringe, value+1, value, ADD, cost); /* rule n->n + 1      */
      if (value != 0){
        fringe = insertValidSucc(fringe, 3*value, value, TRIPLE, cost); /* rule n->3*n        */
        fringe = insertValidSucc(fringe, 2*value, value, DOUBLE, cost); /* rule n->2*n        */
      }
    }
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);  
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);

  /* Initializing the visited array, not sure about the size though... */
  if (start < goal)
    for (int i = start / 3; i < 3 * goal && i < RANGE; i++)
      visited[i] = -1;
  else
    for (int i = goal / 3; i < 3 * start && i < RANGE; i++)
      visited[i] = -1;

  search(fringetype, start, goal); 
  return EXIT_SUCCESS;
}

void printRoute(int target, int start){

  int current, x, length = 0, cost = 0;
  operation op;
  State s;
  Fringe operations = makeFringe(STACK);
  current = target;

  /* Checking what operations were done to get to the target value */
  while (current != start){
    s.value = operationToGetTo[current];
    /* Put the operations on a stack to print in the right order */
    operations = insertFringe(operations, s);
    current = visited[current];
    length++;
  }

  printf("\n%d ", start);
  x = start;
  while (!isEmptyFringe(operations)){
    operations = removeFringe(operations, &s);
    op = s.value;
    cost += costOf(op);
    switch(op){
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
      case NUL:
        printf("NULL ");
        break;
    }
  }
  printf("\n");
  printf("length: %d, cost: %d\n\n", length, cost);
  deallocFringe(operations);
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
