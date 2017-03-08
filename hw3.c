/*
 Emanuil Dobrev
 CS 411
 HW6i
 Alpha beta search
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// board state, contains the utility values of each action
struct state {
  char board[9];
  char values[9];
};

// number of nodes expanded
int nodes = 0;

int getActions(struct state *, int[]);
int alpha_beta_search(struct state *);
int TerminalTest(struct state *);
int Utility(struct state*);
int Max(int, int);
int Min(int, int);
struct state *Result(struct state *, int, char);
int MaxValue(struct state *, int, int);
int MinValue(struct state *, int, int);


// get all possible actions - each action is the index of a blank tile
int getActions(struct state *s, int actions[]) {
  int i = 0;
  int cnt = 0;
  for (i = 0; i < 9; i++) {
    if(s->board[i] == 'b')
      actions[cnt++] = i;
  }
  return cnt;
}

// perform alpha_beta_search
// return the index of the move with highest utility
int alpha_beta_search(struct state *init_state) {
  int v = MaxValue(init_state, -INT_MAX, INT_MAX);
  int i;

  for(i = 0; i < 9; i++) {
    if (init_state->values[i] == v)
      return i;
  }
  return -1;
}

// check if board is full
int TerminalTest(struct state *s) {
  int i = 0;
  for(i = 0; i < 9; i++ ) {
    if(s->board[i] == 'b')
      return 0;
  }
  return 1;
}


// return the utility of a state
// returns higher utility for states that are preferable by X
// and lower utility for states that are preferable by O
int Utility(struct state *s) {
  char p = 'X';
  char e = 'O';

  // check diagonals
  if (s->board[4] == p) {
    if (s->board[0] == p && s->board[8] == p)
      return 4;
    if(s->board[2] == p && s->board[6] == p)
      return 4;
  }  

  if (s->board[4] == e) {
    if (s->board[0] == e && s->board[8] == e)
      return 0;
    if(s->board[2] == e && s->board[6] == e)
      return 0; 
  }

  // check horizontal lines
  int i;
  for(i = 0; i < 9; i += 3) {
    if (s->board[i] == s->board[i+1] == s->board[i+2]) {
      if (s->board[i] == p)
        return 4;
      return 0;
    }
  }

  // check vertical lines
  for(i = 0; i < 3; i++) {
    if(s->board[i] == s->board[i+3] == s->board[i+6]) {
      if(s->board[i] == p)
        return 4;
      return 0;
    }
  }

  if (s->board[4] == 'O')
    return 1;
  if (s->board[4] == 'X')
    return 3;
  return 2;
}

// get the max of 2 ints
int Max(int x, int y) {
  return x > y ? x : y;
}

// get the min of 2 ints
int Min(int x, int y) {
  return x < y ? x : y;
}


// return a new state that results from applying action to s
struct state * Result(struct state *s, int action, char p) {
   struct state *newState = (struct state *)malloc(sizeof(struct state));
   nodes++;
   int i;
   for(i=0; i<9; i++) {
     newState->board[i] = s->board[i];
   }
   newState->board[action] = p;
   return newState;
}

// get the max utility value for state s
int MaxValue(struct state *s, int alpha, int beta) {
  if (TerminalTest(s) )
    return Utility(s);
  int v = -INT_MAX;
  int actions[9];
  int cnt = getActions(s, actions);
  int i;
  for( i = 0; i < cnt; i++) {
    v = Max(v, MinValue(Result(s, actions[i], 'X'), alpha, beta));
    s->values[actions[i]] = v;
    if (v >= beta) return INT_MAX;
    alpha = Max(alpha, v);
  }
  return v;
}

// get the min utility value for state s
int MinValue(struct state *s, int alpha, int beta) {
  if (TerminalTest(s) )
    return Utility(s);
  int v = INT_MAX;
  int actions[9];
  int cnt = getActions(s, actions);
  int i;
  for( i=0; i < cnt; i++) {
    v = Min(v, MaxValue(Result(s, actions[i], 'O'), alpha, beta));
    s->values[actions[i]] = v;
    if (v <= alpha) return -INT_MAX;
    beta = Min(beta, v);
  }
  return v;
}

// display the board  state in a 3x3 matrix
void PrintState(struct state *s) {
  int i;
  for(i = 0; i < 9; i++) {
    printf("%c ", s->board[i]);
    if ( (i+1) % 3 == 0  )
      printf("\n");
  }
}

// display the utility values for each action of a state
// in a 3x3 matrix
void PrintValues(struct state *s) {
  int i;
  for(i = 0; i < 9; i++) {
    printf("%d ", s->values[i]);
    if ( (i+1) % 3 == 0)
      printf("\n");
  }
}

// resets the utility values for each action to -1
void resetValues(struct state *s) {
  int i;
  for(i = 0; i < 9; i++) 
    s->values[i] = -1;
}

// takes a string in the form of "b,b,b,b,b,b,b,b,b"
// and add the board state to init
void parseString(struct state *init, char *s, int size) {
  int i;
  int cnt = 0;
  for(i = 0; i < size; i++) {
    if (s[i] == 'b' || s[i] == 'O' || s[i] == 'X')
      init->board[cnt++] = s[i];
  }
}

// main funciton
int main() {
  // allocate state
  struct state *init = (struct state *) malloc(sizeof(struct state) );
  int i;
  
  char input[100];

  // get user input
  printf("Enter the board state in this format: X,b,b,O,b,b,X,O,b\n");
  scanf("%s", input);

  // parse string and display initial state
  parseString(init, input, strlen(input));
  printf("Initial state: \n"); \
  PrintState(init);
  // perform alpha beta pruning search and get the index of the move with highest utility
  int x = alpha_beta_search(init);
  // error checking
  if (x < 0 || x > 8 ) {
    printf("unexpected error, exiting\n");
    return 0;
  }

  if (init->board[x] != 'b') {
    printf("unexpected error, exiting\n");
  }

  init->board[x] = 'X';
  printf("Optimal move is %d\n", x);
  printf("Nodes expanded: %d\n", nodes);
  printf("Board after move\n");
  PrintState(init);
}

