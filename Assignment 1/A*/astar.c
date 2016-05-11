#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fringe.h"
#include "state.h"

#define N 500  /* N times N chessboard */
#define UNKNOWN 0
#define DISCOVERED 1
#define VISITED 2

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
int costShortestPath[N][N];
int fScore[N][N];
int status[N][N];
unsigned long statesVisited = 0;

int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N);
}

void initialize() {
  int r, c;
  statesVisited = 0;
  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      costShortestPath[r][c] = 999999; /* represents infinity */
      fScore[r][c] = 999999;
      status[r][c] = UNKNOWN;
    }
  }
}

int max(int x, int y){
  if (x > y) return x;
  return y;
}

int min(int x, int y){
  if (x < y) return x;
  return y;
}

/* Heuristic 1 */
int h1(int row, int column, int rowGoal, int columnGoal){
  int deltaRow, deltaColumn;
  int M, m;

  deltaRow = abs(rowGoal - row);
  deltaColumn = abs(columnGoal - column);

  M = max(deltaRow, deltaColumn);
  m = min(deltaRow, deltaColumn);

  /* Too far away, Knight can move max 3 squares per turn,
     so it would have to make at least (M + m) / 3 moves     */
  if (M + m >= 6)
    return ceil((float) (M + m) / 3);

  /* Right spot */
  if (M + m == 0)
    return 0;

  /* Close enough, knight can move 3 squares in any shape */
  return abs(M + m - 3) + 1;
}

/* Heuristic 2 */
int h2(int row, int column, int rowGoal, int columnGoal){
  int deltaRow, deltaColumn;
  int M, m;

  deltaRow = abs(rowGoal - row);
  deltaColumn = abs(columnGoal - column);

  M = max(deltaRow, deltaColumn);
  m = min(deltaRow, deltaColumn);

  /* Too far away, Knight can move max 3 squares per turn,
     so it would have to make at least (M + m) / 3 moves     */
  if (M + m >= 6)
    return 2;

  /* Right spot */
  if (M + m == 0)
    return 0;

  /* Close enough, knight can move 3 squares AWAY from its current position in any shape */
  return abs(M + m - 3) + 1;
}

/* Implements the A* algorithm. Receives a function to be used as a heuristic */
int aStar(int row, int column, int rowGoal, int columnGoal, int (*heuristic) (int, int, int, int)) {

  State s;
  Fringe fringe = makeFringe(PRIO);

  initialize();

  fScore[row][column] = heuristic(row, column, rowGoal, columnGoal);
  costShortestPath[row][column] = 0;

  s.row = row;
  s.column = column;
  fringe = insertFringe(fringe, s, fScore[row][column]);

  while (!isEmptyFringe(fringe)) {
    fringe = removeFringe(fringe, &s);
    statesVisited++;

    /* Goal found? */
    if (s.row == rowGoal && s.column == columnGoal){
      deallocFringe(fringe);
      return costShortestPath[s.row][s.column];
    }

    /* Insert neighbors */
    for (int act = 0; act < 8; act++){
      int r = s.row + actions[act][0];
      int c = s.column + actions[act][1];

      if (isValidLocation(r, c) && status[r][c] != VISITED){
        int gScore = costShortestPath[s.row][s.column] + 1;

        if (gScore < costShortestPath[r][c]){ /* Path better than the previous one? */
          costShortestPath[r][c] = gScore;
          fScore[r][c] = costShortestPath[r][c] + heuristic(r, c, rowGoal, columnGoal);
        }

        if (status[r][c] == UNKNOWN){
          State newState;
          newState.row = r;
          newState.column = c;
          status[r][c] = DISCOVERED;
          fringe = insertFringe(fringe, newState, fScore[r][c]);
        }
      }
      status[s.row][s.column] = VISITED;
    }
  }
  deallocFringe(fringe);
  return 0;
}

double effectiveBranchingFactor(unsigned long states, int d) {
  /* approximates such that N=\Sum+{i=1}^{d} b^i */
  double lwb = 1;
  double upb = pow(states, 1.0/d);
  while (upb - lwb > 0.000001) {
    double mid = (lwb + upb) / 2.0;
    /* the following test makes use of the geometric series */
    if (mid*(1-pow(mid, d))/(1-mid) <= states) {  
      lwb = mid;
    } else {
      upb = mid;
    }
  }
  return lwb;
}

int main(int argc, char *argv[]) {
  int x0,y0, x1,y1;
  do {
    printf("Start location (x,y) = "); fflush(stdout);
    scanf("%d %d", &x0, &y0);
  } while (!isValidLocation(x0,y0));
  do {
    printf("Goal location (x,y)  = "); fflush(stdout);
    scanf("%d %d", &x1, &y1);
  } while (!isValidLocation(x1,y1));

  printf("\nHeuristic 1:\n");
  printf("Length shortest path: %d\n", aStar(x0,y0, x1,y1, h1));
  printf("#visited states: %lu\n", statesVisited);
  printf("#effective branching factor: %f\n", effectiveBranchingFactor(statesVisited, 8));

  printf("\nHeuristic 2:\n");
  printf("Length shortest path: %d\n", aStar(x0,y0, x1,y1, h2));
  printf("#visited states: %lu\n", statesVisited);
  printf("#effective branching factor: %f\n", effectiveBranchingFactor(statesVisited, 8));
  return 0;
}
