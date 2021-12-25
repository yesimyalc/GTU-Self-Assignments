#include <iostream>
#include "helper.h"

using namespace std;

int main()
{
    int boardSize;//This will store the hex game's board's size.
    char GameMethod;//This will store the info that determines if the game will be played with two users or with the computer.
    int numberInput;//This will store the info of the x coordinate of the cell that is choosen by a player or computer.
    int numberInput2;//This will store the info of the y coordinate of the cell that is choosen by the computer.
    char charInput;//This will store the info of the y coordinate of the cell that is choosen by a player.
    chosenMove playerMove;//This will store the coordinates of a cell that a player chose as a move.
    int winCondition=0;//This will determine the win status. If it is equal to 0, it means no one has won yet.

    cout<<"Welcome to the Hex Game."<<endl<<
    "Please enter your desired board size."<<endl<<
    "The board size should be between 6 and 12.(The size you enter will create a board which is (size)x(size).)"<<endl;
    cin>>boardSize;

    while(boardSize<6 || boardSize>12)//Checking the board size info that we get from the user. If it is not a valid board size, it will repeat the process.
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(256,'\n');
        }
        cout<<"You have entered an invalid size. Please enter again."<<endl;
        cin>>boardSize;
    }

    cin.clear();
    cin.ignore(256,'\n');

    char board[12][12];//This will be the hex game's board's array. It determines each cell's coordinates and stores their contents.
    int i;
    int j;
    for(i=0; i<boardSize; ++i)
        for(j=0; j<boardSize; ++j)
            board[i][j]='.';

    cout<<endl<<"Do you want to play with two players(p) or play with the computer(c)? (p/c)"<<endl;
    cin>>GameMethod;
    while(GameMethod!='p' && GameMethod!='c')//Checking if the game method is a valid one.(u=two users/c=with computer)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(256,'\n');
        }
        cout<<"You have entered an invalid game method. Please enter again."<<endl;
        cin>>GameMethod;
    }

    cin.clear();
    cin.ignore(256,'\n');

    if(GameMethod=='p')
    {
        cout<<endl<<"The match begins."<<endl<<
        "Player 1 is using 'X' and Player 2 is using 'O'."<<endl<<
        "Player 1 needs to connect very left and very right sides, player 2 needs to connect top and bottom."<<endl<<
        "You can determine your move by entering dot coordinates. (example: F1, B1, A5)"<<endl<<
        "You can move to east, west, northeast, northwest, southeast or southwest."<<endl;

        DisplayBoard(board, boardSize);

        while(winCondition==0)
        {
            cout<<endl<<"Player 1's Turn"<<endl<<
            "Choose your move: "<<endl;
            cin>>charInput>>numberInput;
            playerMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(playerMove.x)-1][(static_cast<int>(playerMove.y))-(static_cast<int>('A'))]='X';//Changing the cell's content to 'X', making it non available for another move.
            cout<<"The move was "<<playerMove.y<<playerMove.x<<endl;
            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'X')==1)
            {
                cout<<endl<<"Player 1 has won."<<endl;
                break;
            }

            cout<<endl<<"Player 2's Turn"<<endl<<
            "Choose your move: "<<endl;
            cin>>charInput>>numberInput;
            playerMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(playerMove.x)-1][(static_cast<int>(playerMove.y))-(static_cast<int>('A'))]='O';//Changing the cell's content to 'O', making it non available for another move.
            cout<<"The move was "<< (playerMove.y) << (playerMove.x) <<endl;
            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'O')==1)//If there is a win condition, exits the loop and the game ends.
            {
                cout<<endl<<"Player 2 has won."<<endl;
                break;
            }
        }
    }

    if(GameMethod=='c')
    {
        int startCondition=0;//Counts the times player has made a move

        cout<<endl<<"The match begins."<<endl<<
        "Computer is using 'X' and Player is using 'O'."<<endl<<
        "Computer needs to connect very left and very right sides, player needs to connect top and bottom"<<endl<<
        "You can determine your move by entering dot coordinates. (example: F1, B1, A5)"<<endl<<
        "You can move to east, west, northeast, northwest, southeast or southwest."<<endl;

        DisplayBoard(board, boardSize);

        while(winCondition==0)
        {
            cout<<endl<<"Computer's Turn"<<endl;

            /*Computer AI starts here. The main logic of it is, choosing cells that are neighbours to the cell that computer's opponent has recently chosen.
            The computers priority is choosing cells that are top or bottom neighbours because the opponent is trying to connect up side to down side.
            By doing like that, the computer tries to block the opponent's paths.*/

            if(startCondition==0)//Because the player has never made a move before, computer makes it's first move according to the board boardSize
            {
                numberInput=boardSize/2;
                numberInput2=0;
                board[numberInput][numberInput2]='X';
            }

            if(startCondition!=0)//Makes a move according to the player's former move.
            {
                if(board[numberInput-1][numberInput2]=='.')
                {
                    board[numberInput-1][numberInput2]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput<<endl;
                }
                else if(board[numberInput+1][numberInput2]=='.')
                {
                    board[numberInput+1][numberInput2]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput+2<<endl;
                }
                else if(board[numberInput-1][numberInput2+1]=='.')
                {
                    board[numberInput-1][numberInput2+1]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+66)<<numberInput<<endl;
                }
                else if(board[numberInput+1][numberInput2-1]=='.')
                {
                    board[numberInput+1][numberInput2-1]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+64)<<numberInput+2<<endl;
                }
                else if(board[numberInput][numberInput2+1]=='.')
                {
                    board[numberInput][numberInput2+1]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+66)<<numberInput+1<<endl;
                }
                else if(board[numberInput][numberInput2-1]=='.')
                {
                    board[numberInput][numberInput2-1]='X';
                    cout<<"The move was "<<static_cast<char>(numberInput2+64)<<numberInput<<endl;
                }
                else
                {
                    for(i=0; i<boardSize; ++i)
                    {
                      for(j=0; j<boardSize; ++j)
                        if(board[i][j]=='.')
                        {
                            board[i][j]='X';
                            numberInput=i;
                            numberInput2=j;
                            cout<<"The move was "<<static_cast<char>(numberInput2+65)<<numberInput<<endl;
                            break;
                        }
                       if(board[i][j]=='X')
                        break;
                    }
                }
            }
            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'X')==1)//If there is a win condition, exits the loop and the game ends.
            {
                cout<<endl<<"Computer has won."<<endl;
                break;
            }

            cout<<endl<<"Player's Turn"<<endl<<
            "Choose your move: "<<endl;
            cin>>charInput>>numberInput;
            playerMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(playerMove.x)-1][(static_cast<int>(playerMove.y))-(static_cast<int>('A'))]='O';//Changing the cell's content to 'O', making it non available for another move.
            cout<<"The move was "<<playerMove.y<<playerMove.x<<endl;

            numberInput=(playerMove.x)-1;
            numberInput2=(static_cast<int>(playerMove.y))-(static_cast<int>('A'));

            startCondition++;

            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'O')==1)//If there is a win condition, exits the loop and the game ends.
            {
                cout<<endl<<"Player has won."<<endl;
                break;
            }
        }
    }

    return 0;
}
