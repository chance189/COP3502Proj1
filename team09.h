#ifndef TEAM09_H_INCLUDED
#define TEAM09_H_INCLUDED

#include "reversi_functions.h"

/*All prototypes here
*Pieces are added as a enumeration defaults are Empty, White or Black (0, 1, 2)
*Booleans are also added as an enumeration (0, 1)
*struct is defined in reversi_functions, it specifies an x and y location
*Size is default 8
**********************************************************
* We want to find the move that turns the most of the opposite piece's color into our piece.
* Soln: Use MinMax trees! Given the state of the board (as a const) return the move most likely to cause success!
* We have to find the length of moves in a direction that gives us the maximum amount of pieces and makes it so the
* opponent has no valid moves/moves that the opponent won't be able to take all our stuff
* Use alpha beta pruning and min_max
*/


//Define a node for Min_Max_Tree
//More negative the quality, better for black
//more positive the quality, better for white
typedef struct team09_Sim_Move{
    position* sim_move; //postion that is tested
    int quality_move; //maintains the attractiveness of move
} team09_Sim_Move;

position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft); //Required function

//Generates the min max tree by simulating all possible outcomes from the current move choice
//Uses this through min max trees
//position* team09Gen_Min_Max(const enum piece board[][SIZE], enum piece mine, int secondsLeft);

//Returns the best possible move by using a min_max and alpha beta pruning
team09_Sim_Move* team09_Best_Move(const enum piece board[][SIZE], enum piece mine, int depth, int alpha, int beta, int max_depth);
team09_Sim_Move* team09_init_start_move(void);
void team09_free_Sim_Move(team09_Sim_Move* move);
position* team09_initPos(int x, int y);
team09_Sim_Move* team09_init_empty_move(void);
void team09_next_to_space(enum piece board[][SIZE], int*black_next_space, int*white_next_space);
void team09_count_safe(enum piece board[][SIZE], int*black_immune, int*white_immune);

#endif // TEAM09_H_INCLUDED
