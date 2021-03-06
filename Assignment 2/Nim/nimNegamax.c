#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1
#define MAXMATCHES 100

#define INFINITY 9999999

typedef struct move {
  int move;
  int valuation;
} Move;

Move transpTable[2][MAXMATCHES + 1];

Move negaMax(int state, int turn) {
  Move best;
  best.valuation = INFINITY;
  best.move = -10;

  if (state == 1) {
    best.valuation = 1;
    return best;
  }

  if (transpTable[turn][state].move == -1){
    for (int move = 1; move <= 3; move++) {
      if (state - move > 0) {
        Move m = negaMax(state - move, 1 - turn);
        if (m.valuation != INFINITY)
          m.valuation *= -1;

        if (m.valuation < best.valuation) {
          best.valuation = m.valuation;
          best.move = move;
        }
      }
    }
    transpTable[turn][state] = best;
  }
  else {
    best = transpTable[turn][state];
  }
  return best;
}

void initialize(int state){
  Move def;
  def.move = -1;
  def.valuation = -1;
  for (int i = 0; i <= state; i++){
    transpTable[MAX][i] = def;
    transpTable[MIN][i] = def;
  }
}

void playNim(int state) {
  int turn = 0;
  
  initialize(state);

  while (state != 1) {
    Move action = negaMax(state, turn);
    printf("%d: %s takes %d\n", state, 
           (turn==MAX ? "Max" : "Min"), action.move);
    state = state - action.move;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
  if ((argc != 2) || (atoi(argv[1]) < 3)) {
    fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    fprintf(stderr, "<number of sticks> must be at least 3!\n");
    return -1;
  }

  playNim(atoi(argv[1]));

  return 0;
}