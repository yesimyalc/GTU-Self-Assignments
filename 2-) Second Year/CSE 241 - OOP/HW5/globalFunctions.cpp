#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include "cell.h"
#include "exceptions.h"
#include "AbstractHex.h"
#include "HexVector.h"
#include "HexArray1D.h"
#include "HexAdapter.h"
#include "globalFunctions.h"

using namespace std;

namespace HexGame_200104004094
{
    //Checks if the given array of hex games' moves can create a valid sequence of moves for a game
    /*The first game in the array's board will be taken as the first move, the second game will be taken as the second move etc.
    If all these moves when combined can create a valid move sequence, this function will return true.*/
    bool is_validSequence(AbstractHex** gameArray, int arraySize)
    {
        //All of the games must have the same height and width
        for(int i=0; i<arraySize; ++i)
            for(int j=i+1; j<arraySize; ++j)
                if(gameArray[i]->getBoardHeight()!=gameArray[j]->getBoardHeight())
                    return false;
        int n;
        if(gameArray[0]->getGameType()=="user vs user")
            n=1;
        else
            n=2;

        for(int i=0; i<arraySize-1; ++i)
        {
            //Number of moves must be in order (ex: first: 3 moves, second: 4 moves)
            if(gameArray[i+1]->numberOfMoves()!=(gameArray[i]->numberOfMoves()+n))
                return false;

            //The turns must be in order (ex: User1->User2)
            if(gameArray[i]->getWhoseTurn()=="User1" && (gameArray[i+1]->getWhoseTurn()!="User2"))
                return false;

            int counter=0;

            try
            {
                for(int j=0; j<gameArray[i+1]->numberOfMoves(); ++j)
                    for(int k=0; k<gameArray[i]->numberOfMoves(); ++k)
                    {
                        //The following board must have the same moves, and it can have 1 or 2 extra moves depending on the game type
                        if(gameArray[i+1]->getMove(j+1)==gameArray[i]->getMove(k+1))
                            break;
                        else if(k==(gameArray[i]->numberOfMoves()-1))
                        {
                            counter++;
                            if(counter>n)
                                return false;
                        }
                    }
            }
            catch(NoMoveException& exc)
            {
                cerr<<exc.what()<<endl;
                exit(-1);
            }
        }

        return true;
    }

    //Checks if the given array of hex games all have valid moves and valid move structures
    /*Checks if the moves in the games exceed the game board. Also checks if the moves could be done in a
    proper game. For example if there are 3 O but 1 X in the board, this cannot be a valid game.*/
    bool is_validGame(AbstractHex** gameArray, int arraySize)
    {
        try
        {
            int xCount;
            int oCount;

            for(int i=0; i<arraySize; ++i)
            {
                xCount=0;
                oCount=0;

                for(int j=0; j<gameArray[i]->numberOfMoves(); ++j)
                {
                    //Checks if the moves exceed the board
                    if((gameArray[i]->getMove(j+1).getCellRow())>(gameArray[i]->getBoardHeight()) || static_cast<int>(gameArray[i]->getMove(j+1).getCellColumn()-'A')>=gameArray[i]->getBoardWidth())
                        return false;
                    if(gameArray[i]->getMove(j+1).getCellState()==CellState::User1orUser)//Counts X and O
                        xCount++;
                    else
                        oCount++;
                }
                //Checks if there proper amounts of X and O on the board
                if(gameArray[i]->getGameType()=="user vs user" && gameArray[i]->getWhoseTurn()=="User1" && oCount!=xCount)
                    return false;
                else if(gameArray[i]->getGameType()=="user vs user" && gameArray[i]->getWhoseTurn()=="User2" && xCount!=oCount+1)
                    return false;
                else if(gameArray[i]->getGameType()=="user vs computer" && xCount!=oCount)
                    return false;
            }
        }
        catch(NoMoveException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }

        return true;
    }
}
