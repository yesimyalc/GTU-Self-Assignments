#ifndef GLOBALFUNCTIONS_H_INCLUDED
#define GLOBALFUNCTIONS_H_INCLUDED
#include "AbstractHex.h"

namespace HexGame_200104004094
{
    //Checks if the given array of hex games' moves can create a valid sequence of moves for a game
    bool is_validSequence(AbstractHex** games, int gameCount);

    //Checks if the given array of hex games all have valid moves and valid move structures
    bool is_validGame(AbstractHex** games, int gameCount);
}
#endif // GLOBALFUNCTIONS_H_INCLUDED
