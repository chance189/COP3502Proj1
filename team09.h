#ifndef TEAM09_H_INCLUDED
#define TEAM09_H_INCLUDED

#include "reversi_functions.h"

//Author: Chance Reimer & Reia Drucker
//Date: 4/11/2019
/*All prototypes here
* Alpha Beta pruning algorithm, uses Weight for the amount of available moves, the ability to take moves away from the other plater,
* And by choosing the move that will win!
*/


//Define a node for Min_Max_Tree
//More negative the quality, better for black
//more positive the quality, better for white
typedef struct team09_Sim_Move{
    position* sim_move; //postion that is tested
    int quality_move; //maintains the attractiveness of move
} team09_Sim_Move;

position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft); //Required function

//Generates the alpha beta test by simulating all possible outcomes from the current move choice up to the max_depth given
//Uses this through min max trees
//Returns the best possible move by using a min_max and alpha beta pruning
team09_Sim_Move* team09_Best_Move(const enum piece board[][SIZE], enum piece mine, int depth, int alpha, int beta, int max_depth);

//Helper function that counts if a piece has a space next to it, if a piece has a space next to it then it gives opponent more moves
void team09_next_to_space(enum piece board[][SIZE], int*black_next_space, int*white_next_space);

//Counts places on either side where the piece cannot be attacked (side pieces mostly)
void team09_count_safe(enum piece board[][SIZE], int*black_immune, int*white_immune);
//Helper for above
int team09_is_safe(enum piece board[][SIZE], int x, int y);


//Inits the struct we created for our tree
team09_Sim_Move* team09_init_start_move(void);
void team09_free_Sim_Move(team09_Sim_Move* move);
team09_Sim_Move* team09_init_empty_move(void);

#endif // TEAM09_H_INCLUDED
