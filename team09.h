#ifndef TEAM09_H_INCLUDED
#define TEAM09_H_INCLUDED

#include "reversi_functions.h"

//All prototypes here
//Pieces are added as a enumeration defaults are Empty, White or Black (0, 1, 2)
//Booleans are also added as an enumeration (0, 1)
//struct is defined in reversi_functions, it specifies an x and y location
//Size is default 8

position* team09Move(const enum piece board[][SIZE], enum piece mine, int secondsleft); //Required function


#endif // TEAM09_H_INCLUDED
