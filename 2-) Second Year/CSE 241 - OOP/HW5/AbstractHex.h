#ifndef ABSTRACTHEX_H_INCLUDED
#define ABSTRACTHEX_H_INCLUDED

#include <string>
#include <vector>
#include "cell.h"

using namespace std;

namespace HexGame_200104004094
{
    class AbstractHex{
    public:
        //Constructors
        AbstractHex();
        AbstractHex(int heightValue, int widthValue);
        AbstractHex(int heightValue, int widthValue, string gameTypeInfo);

        virtual ~AbstractHex(){}

        //Overloaded Operators
        virtual bool operator==(const AbstractHex& other)const=0;
        virtual char operator()(int indexi, int indexj)const=0;

        //Setters
        void setHexGame(int newHeight, int newWidth, string newGameType);
        void setSize(int newHeight, int newWidth);
        virtual void setCommand()=0;
        virtual void setBoard()=0;

        //Getters
        int getBoardHeight()const{return height;}
        int getBoardWidth()const{return width;}
        const string& getGameType()const{return gameType;}
        const string& getWhoseTurn()const{return whoseTurn;}
        static const string& getCommand(){return command;}
        int numberOfMoves()const{return moveCount;}
        const Cell& lastMove();//Throws NoMoveException
        const Cell& getMove(int moveNo);//Throws NoMoveException

        //Other Functions
        void gameMenu();//Goes to the game menu when MENU command is used
        virtual void reset();//Resets the game to the beginning
        virtual void print()const=0;//Prints the game board
        virtual void readFromFile(string file)=0;
        virtual void writeToFile(string file)const=0;
        virtual void controlCell(int chosenRow, char chosenColum)=0;//Checks if the cell input for move is valid
        virtual const Cell& play()=0;//Plays one step for the computer
        virtual const Cell& play(Cell cMove)=0;//Plays one step for the user
        virtual void playGame()=0;//Starts playing the game
        virtual bool isEnd()const=0;//Checks if there is a win condition

        static int currentGameNo;//Stores the current game's game number
        static int gameCount;//Stores how many games are opened in total
        static string command;

    protected:
        int height;
        int width;
        string gameType;//user vs user or user vs computer
        string whoseTurn="Start";
        int moveCount=0;
        vector<Cell> moves;//Contains the past moves

        //Validity Check Functions
        void checkHexGame();//Controls if the user inputs are valid for the hex game

    };
}
#endif // ABSTRACTHEX_H_INCLUDED
