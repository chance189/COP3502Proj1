/*
* Authors: Chance Reimer
* Purpose: Implement reversi game
*/
#include "team09.h"
#include <stdlib.h>
#include <limits.h>
#include "reversi_functions.h"

//Should return a valid position to play next on the board
position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft)
{
    //Max depth
    int max_depth;
    //Evaluate the empty spaces on the board
    int count_Me = count(board, mine);
    int count_opp = count(board, opposite(mine));

    //Initialize our MinMax end result
    team09_Sim_Move* best_move;
    position* best_move_position;

    //If our min_max is approaching the end don't go too far
    if((count_Me+count_opp) >= 59)
        max_depth = (SIZE*SIZE) - (count_Me+count_opp);
    else
        max_depth = 5;

    best_move = team09_Best_Move(board, mine, 0, INT_MAX, INT_MIN, max_depth);
    best_move_position = best_move->sim_move;
    free(best_move); //Frees the memory allocated, but does not free position
    return best_move_position;
}


/*********
* Returns the best move position available by the team color that is selected
* Uses min_max recursive principle
*/
team09_Sim_Move* team09_Best_Move(const enum piece board[][SIZE], enum piece mine,  int depth, int alpha, int beta, int max_depth)
{
    int no_Moves_Weight = 30;
    int legal_Moves_Weight = 20;

    enum piece tempBoard[SIZE][SIZE]; //Lets initialize a board, it is a local variable thus no need to free
    copy(tempBoard, board); //Copy the board into it
    int value = (mine == BLACK) ? -1 : 1;

    team09_Sim_Move* sim_Move;
    team09_Sim_Move* bestMove = team09_init_start_move();
    team09_free_Sim_Move(bestMove); //deallocate meme used

    int num_Valid_Moves, opp_Valid_Moves, i, game_Eval, no_moves = 0;
    position* opponent_valid;
    position* valid_positions = getPossibleMoves(board, mine, &num_Valid_Moves); //Grab all available moves
    for(i = 0; i < num_Valid_Moves; i++)
    {
        //Since we know that the valid moves are in valid_positions we iterate over them
        //For each available ones, we should make the move
        sim_Move = team09_init_empty_move();
        sim_Move->sim_move = &valid_positions[i];
        executeMove(tempBoard, sim_Move->sim_move, mine);

        //After executing the move, get the total score value, which is more, white or black?
        game_Eval = count(tempBoard, WHITE) - count(tempBoard, BLACK);

        //Evaluate moves available to opponent
        enum piece Opponent = opposite(mine); //Get opposite color
        opponent_valid = getPossibleMoves(tempBoard, mine, &opp_Valid_Moves); //Obtain opponent valid Moves
        free(opponent_valid); //We don't really care what the moves are
        //Check if the opponent has no moves
        //Initialize boolean for determining the end of game
        enum boolean gameOver_local;
        if(opp_Valid_Moves == 0)
        {
            //There are no valid moves, so we increment our counter
            no_moves = value;
            gameOver_local = gameOver(tempBoard);
        }

        //Make check for if the game ended or if our Min Max is deep enough
        if(depth == max_depth || gameOver_local == TRUE)
        {
            if(gameOver_local)
            {
                if(score > 0) //If black wins, make this the maximum minimum
                    sim_Move->quality_move = INT_MAX;
                else if(score < 0)
                    sim_Move->quality_move = INT_MIN;
                else
                    sim_Move->quality_move = 0;
            }
            else //We reached max depth, rate this move
            {
                sim_Move->quality_move = (no_Moves_Weight*no_moves)+(legal_Moves_Weight*(value*(num_Valid_Moves-opp_Valid_Moves)))+game_Eval;
            }
        }
        else //Recurse
        {
            team09_Sim_Move* move_forward = team09_Best_Move(tempBoard, Opponent, depth+1, alpha, beta, max_depth);
            sim_Move->quality_move = move_forward->quality_move;
            team09_free_Sim_Move(move_forward); //We only really care about how good this move is

            //Keep track of if our opponent has no moves
            if(no_moves != 0)
            {
                sim_Move->quality_move += no_Moves_Weight*no_moves;
            }

            //Alpha beta pruning concept here
            if(sim_Move->quality_move > beta && mine == BLACK)
                beta = sim_Move->quality_move;
            else if(sim_Move->quality_move < alpha && mine == WHITE)
                alpha = sim_Move->quality_move;
        }

        if(bestMove->sim_move->x == -1) //test if this is our first
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = sim_Move->quality_move;
        }
        else if((value*bestMove->quality_move) < (value*sim_Move->quality_move))
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = sim_Move->quality_move;
        }
        free(sim_Move); //requires only 1 free since we will free the valid array
    }
    free(valid_positions);
    return bestMove;
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

team09_Sim_Move* team09_init_empty_move(void)
{
    team09_Sim_Move* move = malloc(sizeof(team09_Sim_Move));
    move->sim_move = NULL;
    move->quality_move = 0;
    return move;
}

void team09_free_Sim_Move(team09_Sim_Move* move)
{
    free(move->sim_move);
    free(move);
}

