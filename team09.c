/*
* Authors: Chance Reimer
* Purpose: Implement reversi game
*/
#include "team09.h"
#include <stdlib.h>
#include <limits.h>
//Should return a valid position to play next on the board
position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft)
{

}


/*********
* Returns the best move position available by the team color that is selected
* Uses min_max recursive principle
*/
position* team09_Best_Move(enum piece board[][SIZE], enum piece mine,  int depth, int alpha, int beta)
{
    enum piece tempBoard[SIZE][SIZE]; //Lets initialize a board, it is a local variable thus no need to free
    copy(tempBoard, board); //Copy the board into it

    team09_Sim_Move* bestMove = team09_init_start_move();
    team09_free_Sim_Move(bestMove); //deallocate meme used

    int i, j;
    position* test_pos;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            //adding
            test_pos = team09_initPos(i, j);
            if(isValidMove(board, test_pos, mine))
            {
                //Evaluate the move if it is valid and if it is check if it is the move with the greatest score
                //Our opponents score will be a negative score so the largest value of that will minimize what they do
            }
            else
            {
                free(test_pos); //Free the position we passed to isValidMove
            }
        }
    }
}

position* team09_initPos(int x, int y)
{
    position* pos = malloc(sizeof(position));
    pos->x = x;
    pos->y = y;
    return pos;
}

//Inits the base move of the Best move function
team09_Sim_Move* team09_init_start_move(void)
{
    team09_Sim_Move* move = malloc(sizeof(team09_Sim_Move));
    move->sim_move = malloc(sizeof(position));
    move->sim_move->x = -1;  //Init to impossible place
    move->sim_move->y = -1;
    move->quality_move = INT_MAX;
    return move;
}

void team09_free_Sim_Move(team09_Sim_Move* move)
{
    free(move->sim_move);
    free(move);
}
