#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "cell.h"
#include "exceptions.h"
#include "AbstractHex.h"

using namespace std;

namespace HexGame_200104004094
{
    int AbstractHex::gameCount=0;
    int AbstractHex::currentGameNo=0;
    string AbstractHex::command="Start";

    AbstractHex::AbstractHex()
        :AbstractHex(6, 6, "user vs computer")
    {/*Intentionally left empty*/}

    AbstractHex::AbstractHex(int heightValue, int widthValue)
        :AbstractHex(heightValue, widthValue, "user vs computer")
    {/*Intentionally left empty*/}

    AbstractHex::AbstractHex(int heightValue, int widthValue, string gameTypeInfo)
    {
        setHexGame(heightValue, widthValue, gameTypeInfo);
        moves.resize(getBoardHeight()*getBoardWidth());
    }

    //Sets the hex game
    void AbstractHex::setHexGame(int newHeight, int newWidth, string newGameType)
    {
        gameType=newGameType;
        setSize(newHeight, newWidth);
    }

    //Sets the board sizes, resets the board if it needs to be reseted
    void AbstractHex::setSize(int newHeight, int newWidth)
    {
        height=newHeight;
        width=newWidth;
        checkHexGame();

        if(numberOfMoves()!=0 || getCommand()=="CHANGE")
            reset();
    }

    //Checks the user inputs for the hex game
    void AbstractHex::checkHexGame()
    {
        //Checking the game type validity
        while(gameType!="user vs user" && gameType!="user vs computer")
        {
            cout<<"You have entered invalid game type, please enter again."<<endl;
            getline(cin, gameType);
        }

        //Checking the height and width validity
        while(height<6 || width<6 || height!=width)
        {
            cout<<"You have entered invalid height and width values, please enter again."<<endl<<
            "Enter height: ";
            cin>>height;
            cin.ignore(1000, '\n');
            if(!cin)
            {
               cin.clear();
               cin.ignore(1000, '\n');
            }
            cout<<"Enter width: ";
            cin>>width;
            cin.ignore(1000, '\n');
            if(!cin)
            {
               cin.clear();
               cin.ignore(1000, '\n');
            }
        }
    }

    //Resets the game to the beginning
    void AbstractHex::reset()
    {
        if(getGameType()=="user vs user")
            whoseTurn="User1";
        else
            whoseTurn="User";

        //Resetting the moves
        moveCount=0;
        //Resizing in case the height and the width values are changed
        moves.resize(getBoardHeight()*getBoardWidth());
    }

    //Returns the Cell that was chosen in the last move
    //Throws NoMoveException if there is no moves
    const Cell& AbstractHex::lastMove()
    {
        if(numberOfMoves()==0)
            throw NoMoveException();

        return moves[numberOfMoves()-1];
    }

    //Returns the move in the given move number
    //Throws NoMoveException if there is no move with the number
    const Cell& AbstractHex::getMove(int moveNo)
    {
        if(numberOfMoves()<moveNo)
            throw NoMoveException();

        return moves[moveNo-1];
    }

    //Goes to game menu if MENU command is used
    void AbstractHex::gameMenu()
    {
        cout<<endl<<"**********Game Menu**********"<<endl;
        int temp;
        cout<<endl<<"There are "<<gameCount<<" games opened. Which game do you want to switch to?"<<endl<<
        currentGameNo<<"-) Current Game"<<endl;
            for(int i=0; i<gameCount; ++i)
                if(i+1!=currentGameNo)
                   cout<<i+1<<"-) Game "<<i+1<<endl;
        temp=currentGameNo;
        cin>>currentGameNo;
        cin.ignore(1000,'\n');
        while(currentGameNo<1 || currentGameNo>gameCount)
        {
            if(!cin)
            {
                cin.clear();
                cin.ignore(1000,'\n');
            }
            cout<<"You have entered an invalid number. Please enter again."<<endl;
            cin>>currentGameNo;
            cin.ignore(1000,'\n');
        }

        if(currentGameNo==temp)
        {
            cout<<"You have entered you old game number. The game will continue from where it was left."<<endl;
            setCommand();
        }
        cout<<endl<<"*****************************"<<endl;
    }
}
