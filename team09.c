/*
* Authors: Chance Reimer
* Purpose: Implement reversi game
*/
#include "team09.h"
#include <stdlib.h>
//Should return a valid position to play next on the board
position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft)
{

}


/*********
* Returns the best move position available by the team color that is selected
* Uses min_max recursive principle
*/
position* team09_Best_Move(const enum piece board[][SIZE], enum piece mine,  int depth, int alpha, int beta)
{
    enum piece tempBoard[SIZE][SIZE]; //Lets initialize a board
    copy(tempBoard, board); //Copy the board into it

    position* bestMove = team09_init_start_move();

    free(bestMove); //deallocate meme used
}

//Inits the base move of the Best move function
position* team09_init_start_move(void)
{
    position* move = malloc(sizeof(position));
    move->x = -1;  //Init to impossible place
    move->y = -1;
    return move;
}
