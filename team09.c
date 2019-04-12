/*
* Authors: Chance Reimer
* Purpose: Implement reversi game
*/
#include "team09.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "reversi_functions.h"

const int team_09DX_SIZE = 8;

//Setup Purposefully so that +4 will give us the opposite direction
const int team_09DX[] = {-1,-1,-1, 0, 1, 1, 1,  0};
const int team_09DY[] = {-1, 0, 1, 1, 1, 0,-1, -1};

//Should return a valid position to play next on the board
position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft)
{
    //Max depth
    int max_depth = 4;

    //Initialize our MinMax end result
    team09_Sim_Move* best_move;  //pointer to grab our result and free its memory
    position* best_move_position; //Ditto

    best_move = team09_Best_Move(board, mine, 0, INT_MIN, INT_MAX, max_depth); //Grab our best Move
    best_move_position = best_move->sim_move;  //Grab the position pointer needed to be returned

    free(best_move); //Frees the memory allocated, but does not free position
    return best_move_position;  //return
}


/*********
* Returns the best move position available by the team color that is selected
* Uses min_max recursive principle
*/
team09_Sim_Move* team09_Best_Move(const enum piece board[][SIZE], enum piece mine,  int depth, int alpha, int beta, int max_depth)
{
    //Assign weights needed to calculate move's attractiveness
    int no_Moves_Weight = 100;  //If we can choose a situation where the opponent can't move, that's good
    int legal_Moves_Weight = 5; //We want to have more moves than the opponent
    int emptySpace = 10; //If the spot contains an empty space, then it is not good
    int no_eat = 50;  //most important to ensure that we make moves that don't allow opponent to eat our pieces

    enum piece tempBoard[SIZE][SIZE]; //Lets initialize a board, it is a local variable thus no need to free
    copy(tempBoard, board); //Copy the board into it

    int Min_Or_Max = (mine == BLACK) ? -1 : 1;  //Make so our Black is negative

    team09_Sim_Move* sim_Move;  //Simulate the next move
    team09_Sim_Move* bestMove = team09_init_start_move();  //Create

    //initialize counter,
    int i, game_Eval, no_moves = 0;
    //Evaluate the pieces that are next to spaces
    int black_Next_Space, white_Next_Space, black_no_eat, white_no_eat;

    //For Opponent Positions
    int opp_Valid_Moves;
    position* opponent_valid;

    //For My Positions
    int num_Valid_Moves;
    position* valid_positions = getPossibleMoves(board, mine, &num_Valid_Moves); //Grab all available moves

    //Since we know that the valid moves are in valid_positions we iterate over them
    for(i = 0; i < num_Valid_Moves; i++)
    {
        no_moves = 0;    //reset this variable to detect interrupts for opponent
        copy(tempBoard, board); //New copy every time for resetting board to original state

        //For each available ones, we should make the move
        sim_Move = team09_init_empty_move();
        sim_Move->sim_move = &valid_positions[i];
        executeMove(tempBoard, sim_Move->sim_move, mine);

        //After executing the move, get the total score value, which is more, white or black?
        game_Eval = count(tempBoard, WHITE) - count(tempBoard, BLACK);

        //Evaluate moves available to opponent
        enum piece Opponent = opposite(mine); //Get opposite color
        opponent_valid = getPossibleMoves(tempBoard, Opponent, &opp_Valid_Moves); //Obtain opponent valid Moves
        free(opponent_valid); //We don't really care what the moves are


        //Check if the opponent has no moves
        //Initialize boolean for determining the end of game
        enum boolean gameOver_local = FALSE;
        gameOver_local = gameOver(tempBoard);
        if(opp_Valid_Moves == 0)
        {
            //There are no valid moves, so we increment our counter
            no_moves = Min_Or_Max;

            //If opponent has no moves, it's my turn again!
            Opponent = mine;
        }

        //Make check for if the game ended or if our Min Max is deep enough
        if(depth == max_depth || gameOver_local == TRUE)
        {
            if(gameOver_local == TRUE)
            {
                if(game_Eval > 0) //If white wins, make this the maximum
                    sim_Move->quality_move = INT_MAX;
                else if(game_Eval < 0) // black wins, make this maximum negative
                    sim_Move->quality_move = INT_MIN;
                else
                    sim_Move->quality_move = 0; //ties
            }
            else //We reached max depth, rate this move
            {
                team09_next_to_space(tempBoard, &black_Next_Space, &white_Next_Space); //populate all pieces touching spaces
                team09_count_safe(tempBoard, &black_no_eat, &white_no_eat);            //populate all pieces that cannot be taken by opponent

                sim_Move->quality_move = no_eat*(white_no_eat-black_no_eat)+ //The value given if the move cannot be attacked
                            (emptySpace*(black_Next_Space-white_Next_Space)) //This is bad, since we want no spaces
                            +(no_Moves_Weight*no_moves) //Want to try to force a forfeit
                            +(legal_Moves_Weight*(Min_Or_Max*(num_Valid_Moves-opp_Valid_Moves)))+game_Eval;
            }
        }
        else //Recurse
        {
            team09_Sim_Move* move_forward = team09_Best_Move(tempBoard, Opponent, depth+1, alpha, beta, max_depth);
            sim_Move->quality_move = move_forward->quality_move;
            team09_free_Sim_Move(move_forward); //We only really care about how good this move is

            //Keep track of if our opponent has no moves
            if(no_moves != 0 && sim_Move->quality_move != INT_MAX && sim_Move->quality_move != INT_MIN)
            {
                sim_Move->quality_move += no_Moves_Weight*no_moves;
            }

            //Alpha beta pruning concept here
            if(sim_Move->quality_move > alpha && mine == WHITE) //Since White is positive (Max) it associates with alpha
                alpha = sim_Move->quality_move;
            else if(sim_Move->quality_move < beta && mine == BLACK) //Since black is min, associates with beta
                beta = sim_Move->quality_move;
        }

        if(bestMove->sim_move->x == -1) //test if this is our first
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = sim_Move->quality_move;
        }
        else if((Min_Or_Max*(bestMove->quality_move)) < (Min_Or_Max*(sim_Move->quality_move)))
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = sim_Move->quality_move;
        }

        //check if we go outside our ranges
        if(sim_Move->quality_move > beta && mine == WHITE)  //test case beta < alpha, we return on this condition
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = beta;
            free(sim_Move); //Release our best move
            free(valid_positions);
            return bestMove;
        }
        else if(sim_Move->quality_move < alpha && mine == BLACK) //test case beta < alpha, we return on this condition
        {
            bestMove->sim_move->x = sim_Move->sim_move->x;
            bestMove->sim_move->y = sim_Move->sim_move->y;
            bestMove->quality_move = alpha;
            free(sim_Move); //Release our best move
            free(valid_positions);
            return bestMove;
        }

        free(sim_Move); //requires only 1 free since we will free the valid array
    }
    free(valid_positions); //Free the positions we grabbed
    return bestMove;       //return with the best move
}

//Looks along each direction to see if a collision with a space or different piece occurs,
//Code is similar to getStreakEnd
void team09_count_safe(enum piece board[][SIZE], int*black_immune, int*white_immune)
{
    int i, j;
    *black_immune = 0;
    *white_immune = 0;

    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            //Know that we can take a piece if we have at least one space
            //on a side of our piece
            if(board[i][j] != EMPTY) //So if we get to a piece, check if it can be taken
            {
                if(team09_is_safe(board, i, j))
                {
                    //get type
                    if(board[i][j] == BLACK)
                        (*black_immune)++;
                    else
                        (*white_immune)++; //Only other case is white
                }
            }
        }
    }

}

//Helper function, determining with our own special DXarray what is in front or behind us
int team09_is_safe(enum piece board[][SIZE], int x, int y)
{
    enum piece type = board[x][y];
    int i;
    position* spot = malloc(sizeof(position));
    spot->x = x;
    spot->y = y;

    //go to next pos
    for(i = 0; i < 4; i++)
    {
        //next pos
        spot->x += team_09DX[i];
        spot->y += team_09DY[i];

        if(!inbounds(spot))  //not in bounds so automatically pass
            continue; //go to next test

        while(board[spot->x][spot->y] == type)
        {
            spot->x += team_09DX[i];
            spot->y += team_09DY[i];
            if(!inbounds(spot))
                break;
        }
        //check if we are not in inbounds
        if(!inbounds(spot)) //check if this is why we broke
            continue; //if yes we are all good
        else
        {
            //test opposite side since we must have reached wrong character
            spot->x = x;
            spot->y = y;
            spot->x += team_09DX[(i+4)%team_09DX_SIZE]; //Move to next available on opposite side
            spot->y += team_09DY[(i+4)%team_09DX_SIZE];
            if(!inbounds(spot))
                continue;

            while(board[spot->x][spot->y] == type)
            {
                spot->x += team_09DX[(i+4)%team_09DX_SIZE];
                spot->y += team_09DY[(i+4)%team_09DX_SIZE];
                if(!inbounds(spot))
                    break;
            }
            //check if we are not in inbounds
            if(!inbounds(spot)) //check if this is why we broke
                continue; //if yes we are all good
            else
            {
                free(spot);
                return 0;
            }
        }
    }
    //We get through all 4 cases! we are good!
    free(spot);
    return 1;
}

//iterates all pieces for either, returning amount of pieces are adjacent to spaces
void team09_next_to_space(enum piece board[][SIZE], int*black_next_space, int*white_next_space)
{
    int i, j, k;
    *black_next_space = 0;
    *white_next_space = 0;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(board[i][j] != EMPTY)
            {
                for(k = 0; k < team_09DX_SIZE; k++)
                {
                    int newI = i+team_09DX[k];
                    int newJ = j+team_09DY[k];
                    if(newI == i && newJ == j)
                    {
                        continue;
                    }
                    else if(newI >= 0 && newI < SIZE && newJ >=0 && newJ < SIZE)
                    {
                        if(board[newI][newJ] == EMPTY && board[i][j] == BLACK)
                           (*black_next_space)+=1;
                        else if(board[newI][newJ] == EMPTY && board[i][j] == WHITE)
                            (*white_next_space)+=1;
                    }
                }
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

