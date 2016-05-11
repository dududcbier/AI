#include <stdio.h>
#include <stdlib.h>

#define N 500   /* N times N chessboard */

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
int costShortestPath[N][N];
unsigned long statesVisited = 0;

int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N);
}

void initialize() {
  int r, c;
  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      costShortestPath[r][c] = 999999; /* represents infinity */
    }
  }
}

int knightDLS(int cost, int limit, int row, int column, int rowGoal, int columnGoal) {
  int act;
  statesVisited++;
  if (row == rowGoal && column == columnGoal) {
    return 1;  /* goal reached */
  }
  if (cost == limit || cost >= costShortestPath[row][column]) {
    return 0; /* limit reached, or we've been here before via a 'cheaper' path */
  }
  costShortestPath[row][column] = cost;
  for (act=0; act < 8; act++) {
    int r = row + actions[act][0];
    int c = column + actions[act][1];
    if (isValidLocation(r, c) && knightDLS(cost+1, limit, r, c, rowGoal, columnGoal) == 1) {
      return 1;
    }
  }
  return 0;
}

int knightIDS(int row, int column, int rowGoal, int columnGoal) {
  int limit = 0;
  printf ("limit=0"); fflush(stdout);
  initialize();
  while (knightDLS(0, limit, row, column, rowGoal, columnGoal) == 0) {
    initialize();
    limit++;
    printf(",%d", limit); fflush(stdout);
  }
  printf("\n");
  return limit;
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

  printf("Length shortest path: %d\n", knightIDS(x0,y0, x1,y1));
  printf("#visited states: %lu\n", statesVisited);
  return 0;
}
