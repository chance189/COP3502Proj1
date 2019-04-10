#include <stdlib.h>
#include <time.h>

#include "team20.h"
#include "reversi_functions.h"

position* team20Move(const enum piece board[][SIZE], enum piece mine, int secondsleft) {

    // For randomness!
    srand(time(0));

    // Get the move list, just choose random one.
    int numMoves;
    position* choices = getPossibleMoves(board, mine, &numMoves);
    int idx = rand()%numMoves;

    // Copy the move into our newly created struct.
    position* res = malloc(sizeof(position));
    res->x = choices[idx].x;
    res->y = choices[idx].y;

    // Free the array.
    free(choices);

    // Return our randomly selected move.
    return res;
}
