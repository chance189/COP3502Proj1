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
*/
position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft); //Required function

//Generates the min max tree by simulating all possible outcomes from the current move choice
//Uses this through min max trees
position* team09Gen_Min_Max(const enum piece board[][SIZE], enum piece mine, int secondsLeft);

//Returns the best possible move of the generated MinMax Tree
position* team09_Best_Move(const enum piece board[][SIZE], enum piece mine, int secondsLeft);

#endif // TEAM09_H_INCLUDED
