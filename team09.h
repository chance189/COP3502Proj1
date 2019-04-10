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
* opponent has no valid moves
* Use alpha beta pruning and min_max
*/
extern int team09_Min_Max_Depth;
extern int weight_forfeit, weight_frontier, weight_mobility, weight_stability;



position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft); //Required function

//Generates the min max tree by simulating all possible outcomes from the current move choice
//Uses this through min max trees
//position* team09Gen_Min_Max(const enum piece board[][SIZE], enum piece mine, int secondsLeft);

//Returns the best possible move by using a min_max
position* team09_Best_Move(enum piece board[][SIZE], enum piece mine, int depth, int alpha, int beta);
position* team09_init_start_move(void);
#endif // TEAM09_H_INCLUDED
