#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "helper.h"

using namespace std;

int main()
{
    int boardSize;
    char board[12][12];
    char GameMethod;//This will store the info that determines if the game will be played with two users or with the computer.
    string gameCondition="new game";
    string whoseTurn="Start";
    int numberInput;//This will store the info of the x coordinate of the cell that is choosen by a player or computer.
    int numberInput2;//This will store the info of the y coordinate of the cell that is choosen by the computer.
    char charInput;//This will store the info of the y coordinate of the cell that is choosen by a player.
    chosenMove playerMove;
    int winCondition=0;//This will determine the win status. If it is equal to 0, it means no one has won yet.
    int i;
    int j;
    string command;
    char turnBack='n';//Will turn back to the beginning of turns if 'y'
    int totalGames=0;
    string fileName;

    for(i=1; i>0; ++i)//Gets how many saved games there are from old runs of the program.
    {
        fileName="SavedGame"+to_string(i)+".txt";
        ifstream file(fileName);

        if(!file)
            break;
        else
        {
            totalGames++;
            file.close();
        }
    }

    cout<<"Welcome to the Hex Game."<<endl<<
    "Do you want to load an old game or start a new game? (new game/old game)"<<endl;
    getline(cin, gameCondition);
    while(gameCondition!="new game" && gameCondition!="old game")
    {
        cout<<"You have entered an invalid game condition. Please enter new game or load game."<<endl;
        getline(cin, gameCondition);
    }

    if(gameCondition=="old game")//If gameCondition is old game, it loads the old game from SavedGames.txt
    {
        loadFile(board, boardSize, GameMethod, whoseTurn, playerMove, totalGames);
        cout<<"The move was "<<playerMove.y<<playerMove.x<<endl;
        DisplayBoard(board, boardSize);
    }

    if(gameCondition=="new game")
    {
        cout<<endl<<"Please enter your desired board size."<<endl<<
        "The board size should be between 6 and 12.(The size you enter will create a board which is (size)x(size).)"<<endl;

        cin>>boardSize;
        cin.ignore(1000,'\n');

        while(boardSize<6 || boardSize>12)//Checking the board size info that we get from the user.
        {
            if(!cin)
            {
               cin.clear();
               cin.ignore(1000,'\n');
            }
            cout<<"You have entered an invalid size. Please enter again."<<endl;
            cin>>boardSize;
            cin.ignore(1000,'\n');
        }

        for(i=0; i<boardSize; ++i)
           for(j=0; j<boardSize; ++j)
              board[i][j]=static_cast<char>(CellState::Empty);

        cout<<endl<<"Do you want to play with two users(u) or play with the computer(c)? (u/c)"<<endl;
        cin>>GameMethod;
        cin.ignore(1000,'\n');
        while(GameMethod!='u' && GameMethod!='c')//Checking if the game method is a valid one.(u=two users/c=with computer)
        {
           if(!cin)
           {
              cin.clear();
              cin.ignore(1000,'\n');
           }
           cout<<"You have entered an invalid game method. Please enter again."<<endl;
           cin>>GameMethod;
           cin.ignore(1000,'\n');
        }

        cout<<endl<<"The match begins."<<endl<<
        "User 1 or Computer is using 'X' and User 2 is using 'O'."<<endl<<
        "User 1 or Computer needs to connect very left and very right sides, User 2 needs to connect top and bottom."<<endl<<
        "You can determine your move by entering dot coordinates. (example: F1, B1, A5)"<<endl<<
        "You can move to east, west, northeast, northwest, southeast or southwest."<<endl<<
        "If you want to save or load the game at any point, you can use LOAD FILE.TXT and SAVE FILE.TXT commands and go to save&load menu."<<endl;

        DisplayBoard(board, boardSize);
    }

    while(winCondition==0)
    {
        if(GameMethod=='u')
        {
            if(whoseTurn=="Start")
               whoseTurn="User1";

            cout<<endl<<whoseTurn<<"'s Turn"<<endl;
            getCommand(command, numberInput, charInput);
            if(command=="SAVE FILE.TXT" || command=="LOAD FILE.TXT")
                menuOperations(command, board, boardSize, GameMethod, whoseTurn, playerMove, turnBack, totalGames);
            playerMove=ValidMoveCheck(board, boardSize, numberInput, charInput, playerMove, command);
            if(command=="SAVE FILE.TXT" || command=="LOAD FILE.TXT")//If the player enters invalid move inputs before and returns with a menu type command from ValidMoveCheck, goes to menu. Otherwise, this operation will not happen.
                menuOperations(command, board, boardSize, GameMethod, whoseTurn, playerMove, turnBack, totalGames);

            if(whoseTurn=="User1" && turnBack=='n')
            {
                makeMove(board, boardSize, whoseTurn, playerMove);

                if(WinCheck(board, boardSize)==1)//If there is a win condition, exits the loop and the game ends.
                {
                   cout<<endl<<"User 1 has won."<<endl;
                   break;
                }
            }
            if(whoseTurn=="User2" && turnBack=='n')
            {
                makeMove(board, boardSize, whoseTurn, playerMove);

                if(WinCheck(board, boardSize, static_cast<char>(CellState::User2))==1)
                {
                   cout<<endl<<"User 2 has won."<<endl;
                   break;
                }
            }

            if(turnBack!='y' && whoseTurn=="User1")//Passes the turn to the next player, if there was no MENU command called.
                whoseTurn="User2";
            else if(turnBack!='y' && whoseTurn=="User2")
                whoseTurn="User1";

            if(turnBack=='y')
                turnBack='n';
        }

       if(GameMethod=='c')
       {
           int startCondition;
           if(whoseTurn=="Start")
            {
                whoseTurn="Computer";
                startCondition=0;//Counts the times player has made a move
            }

            if(whoseTurn=="Computer")
            {
                cout<<endl<<"Computer's Turn"<<endl;

                //Computer AI starts here.
                if(startCondition==0)//Because the player has never made a move before, computer makes it's first move according to the board boardSize
                {
                   numberInput=boardSize/2;
                   numberInput2=0;
                   board[numberInput][numberInput2]=static_cast<char>(CellState::User1orComputer);
                }
                if(startCondition!=0)//Makes a move according to the user's former move. It's priority is to block the user's paths.
                {
                   if(board[numberInput-1][numberInput2]==static_cast<char>(CellState::Empty))//Checks the first priority cell, if available chooses it.
                   {
                       board[numberInput-1][numberInput2]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput<<endl;
                   }
                   else if(board[numberInput+1][numberInput2]==static_cast<char>(CellState::Empty))//Checks the second priority cell, if available chooses it. Goes on like this.
                   {
                       board[numberInput+1][numberInput2]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput+2<<endl;
                   }
                   else if(board[numberInput-1][numberInput2+1]==static_cast<char>(CellState::Empty))
                   {
                       board[numberInput-1][numberInput2+1]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+66)<<numberInput<<endl;
                   }
                   else if(board[numberInput+1][numberInput2-1]==static_cast<char>(CellState::Empty))
                   {
                       board[numberInput+1][numberInput2-1]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+64)<<numberInput+2<<endl;
                   }
                   else if(board[numberInput][numberInput2+1]==static_cast<char>(CellState::Empty))
                   {
                       board[numberInput][numberInput2+1]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+66)<<numberInput+1<<endl;
                   }
                   else if(board[numberInput][numberInput2-1]==static_cast<char>(CellState::Empty))
                   {
                       board[numberInput][numberInput2-1]=static_cast<char>(CellState::User1orComputer);
                       cout<<"The move was "<<static_cast<char>(numberInput2+64)<<numberInput<<endl;
                   }
                   else//Makes a move to the first empty cell that it finds if all the neighbours of the move of the user did are not empty.
                   {
                      for(i=0; i<boardSize; ++i)
                      {
                        for(j=0; j<boardSize; ++j)
                          if(board[i][j]==static_cast<char>(CellState::Empty))
                          {
                              board[i][j]=static_cast<char>(CellState::User1orComputer);
                              numberInput=i;
                              numberInput2=j;
                              cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput<<endl;
                              break;
                          }
                         if(board[i][j]==static_cast<char>(CellState::User1orComputer))
                          break;
                       }
                    }
                 }
                 //Computer AI ends here.
                 DisplayBoard(board, boardSize);

                if(WinCheck(board, boardSize)==1)
                {
                  cout<<endl<<"Computer has won."<<endl;
                  break;
                }

                whoseTurn="User";
            }

            cout<<endl<<"User's Turn"<<endl;
            getCommand(command, numberInput, charInput);
            if(command=="SAVE FILE.TXT" || command=="LOAD FILE.TXT")
                menuOperations(command, board, boardSize, GameMethod, whoseTurn, playerMove, turnBack, totalGames);
            playerMove=ValidMoveCheck(board, boardSize, numberInput, charInput, playerMove, command);
            if(command=="SAVE FILE.TXT" || command=="LOAD FILE.TXT")//If the player enters invalid move inputs before and returns with a MENU command from ValidMoveCheck, goes to MENU. Otherwise, this operation will not happen.
                menuOperations(command, board, boardSize, GameMethod, whoseTurn, playerMove, turnBack, totalGames);

            if(whoseTurn=="User" && turnBack=='n')
            {
                makeMove(board, boardSize, whoseTurn, playerMove);

                //Converts the move information to informations that the computer AI will use.
                numberInput=(playerMove.x)-1;
                numberInput2=((decltype(playerMove.x)(playerMove.y))-(static_cast<int>('A')));

                startCondition++;

                if(WinCheck(board, boardSize, static_cast<char>(CellState::User2))==1)
                {
                   cout<<endl<<"User has won."<<endl;
                   break;
                }
                whoseTurn="Computer";
            }

            if(turnBack=='y')
                turnBack='n';
        }
    }
    return 0;
}
