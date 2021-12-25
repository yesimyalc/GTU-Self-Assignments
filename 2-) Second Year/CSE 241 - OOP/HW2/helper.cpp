#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "helper.h"

using namespace std;

//This function displays the hex game's board.
void DisplayBoard(const char (&boardArray)[12][12], int boardSize)
{
    int l;

    for(int i=0; i<boardSize; ++i)
      for(int j=0; j<boardSize; ++j)
      {
        if(i==0 && j==0)
        {
            cout<<"  ";
            for(l=0; l<boardSize; l++)
              cout<<" "<<static_cast<char>('A'+l);
        }

        if(j==0)
        {
            cout<<endl<<i+1;
              for(l=0; l<i+1; ++l)
                cout<<" ";
        }
        cout<<" "<<boardArray[i][j];
       }
}

//This function gets the user command for cell moves or for going to the menu.
void getCommand(string& inputCommand, int& nInput, char& cInput)
{
    string numberPart;
    cout<<"Choose your move: "<<endl;
    getline(cin, inputCommand);

    if(inputCommand!="SAVE FILE.TXT" && inputCommand!="LOAD FILE.TXT" && inputCommand.size()!=1)//If input command is not one of these, it means it is either an invalid one or a cell input. Converts the command to a proper cell input.
    {
        cInput=decltype(cInput)(inputCommand.front());
        numberPart=inputCommand.substr(1);
        for(int i=0; i<numberPart.size(); ++i)
        {
            if(numberPart[i]>'9' || numberPart[i]<'0')//If the number part of the cell input is not valid, it makes it -1 to be able to change it in validMoveCheck function later on.
            {
                numberPart="-1";
                break;
            }
        }
        nInput=stoi(numberPart);
    }
    else if(inputCommand!="SAVE FILE.TXT" && inputCommand!="LOAD FILE.TXT" && inputCommand.size()==1)
    {
        cInput=inputCommand.front();
        nInput=-1;
    }
}

//This function goes to load&save menu and does operations accordingly
void menuOperations(const string& inputCommand, char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, char& backCommand, int& totalGameNumber)
{
    if(inputCommand=="SAVE FILE.TXT")
    {
        saveFile(boardArray, boardSize, currentMethod, currentTurn, currentMove, totalGameNumber);
        DisplayBoard(boardArray, boardSize);
    }
    else if(inputCommand=="LOAD FILE.TXT")
    {
        loadFile(boardArray, boardSize, currentMethod, currentTurn, currentMove, totalGameNumber);

        cout<<endl<<"The move was "<<currentMove.y<<currentMove.x<<endl;
        DisplayBoard(boardArray, boardSize);
    }

    backCommand='y';
}

//This function saves the game.
void saveFile(char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, int& totalGameNumber)
{
    ofstream writeGame;
    ofstream writeGameCount;
    string fileName;
    int saveSlot;

    cout<<"There are "<<totalGameNumber<<" games saved."<<endl<<
    "Choose the slot you want to save your game into. (example: 1, 2, 3, ...)"<<endl<<
    "If you want to overwrite any game, you can enter their slot number otherwise you can enter the following slot number."<<endl;
    cin>>saveSlot;
    cin.ignore(1000,'\n');

    //Does not let the user choose any negative slot inputs. Also the user has to either choose an already existed slot or the following slot.
    //For example the user should not choose 5th slot if there is no 4th slot.
    while(saveSlot>(totalGameNumber+1) || saveSlot<1)
    {
        if(!cin)
        {
           cin.clear();
           cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid slot number. Please enter again."<<endl;
        cin>>saveSlot;
        cin.ignore(1000,'\n');
    }

    if(saveSlot>totalGameNumber)//If there was no overwriting, total saved game number increases.
        totalGameNumber++;

    fileName="SavedGame"+to_string(saveSlot)+".txt";

    writeGame.open(fileName);
    if(writeGame.fail())
    {
        cerr<<"File cannot be opened."<<endl;
        exit(-1);
    }

    writeGame<<boardSize<<endl<<currentMethod<<endl<<currentTurn<<endl<<currentMove.y<<currentMove.x<<endl;
    for(int i=0; i<boardSize; ++i)
        for(int j=0; j<boardSize; ++j)
           writeGame<<boardArray[i][j];

    writeGame.close();
}

//This function loads the game.
void loadFile(char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, int& totalGameNumber)
{
    ifstream getGame;
    string fileName;
    int saveSlot;

    if(totalGameNumber==0)
    {
        cerr<<"There are no files to load from."<<endl;
        exit(-1);
    }

    cout<<"There are "<<totalGameNumber<<" games saved."<<endl<<
    "Choose the slot you want to load your game from. (example: 1, 2, 3, ...)"<<endl;
    cin>>saveSlot;
    cin.ignore(1000,'\n');

    //Does not let the user chose slots that have not been created before.
    while(saveSlot>totalGameNumber || saveSlot<1)
    {
        if(!cin)
        {
           cin.clear();
           cin.ignore(1000,'\n');
        }

        cout<<"You have entered an invalid slot number. Please enter again."<<endl;
        cin>>saveSlot;
        cin.ignore(1000,'\n');
    }

    fileName="SavedGame"+to_string(saveSlot)+".txt";

    getGame.open(fileName);
    if(getGame.fail())
    {
        cerr<<"Any saved game files cannot be opened."<<endl;
        exit(-1);
    }

    getGame>>boardSize>>currentMethod>>currentTurn>>currentMove.y>>currentMove.x;
    for(int i=0; i<boardSize; ++i)
        for(int j=0; j<boardSize; ++j)
           getGame>>boardArray[i][j];

    getGame.close();
}

//This function makes the player moves
void makeMove(char (&boardArray)[12][12], int boardSize, const string& currentTurn, const chosenMove& currentMove)
{
    CellState currentState;
    //Decides what to fill the cell with. X or O.
    if(currentTurn=="User1")
        currentState=(CellState::User1orComputer);
    else
        currentState=(CellState::User2);

    boardArray[(currentMove.x)-1][(decltype(currentMove.x)(currentMove.y))-(static_cast<int>('A'))]=static_cast<char>(currentState);
    cout<<"The move was "<<currentMove.y<<currentMove.x<<endl;
    DisplayBoard(boardArray, boardSize);
}

//This function checks if the move of a player was a legal move.
chosenMove ValidMoveCheck(const char (&boardArray)[12][12], int boardSize, int nInput, char cInput, chosenMove oldMove, string& inputCommand)
{
    int validCheck=0;
    chosenMove validMove;

    //Checking if the command is a valid one
    if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]==static_cast<char>(CellState::Empty)))
        validCheck=1;
    if(inputCommand=="SAVE FILE.TXT" || inputCommand=="LOAD FILE.TXT")//If the command was MENU, the function returns the former move because it will be used in menu operations.
    {
        validMove.x=oldMove.x;
        validMove.y=oldMove.y;
        inputCommand=" ";
        return validMove;
    }

       while(validCheck==0)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(100,'\n');
        }
        cout<<"Move you wanted to do was not legal, please make another move."<<endl;
        getCommand(inputCommand, nInput, cInput);
        if(inputCommand=="SAVE FILE.TXT" || inputCommand=="LOAD FILE.TXT")//If the newly inputed commad is MENU, the function returns the former move because it will be used in menu operations.
        {
            validMove.x=oldMove.x;
            validMove.y=oldMove.y;
            return validMove;
        }

        if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]==static_cast<char>(CellState::Empty)))
           validCheck=1;
    }

    validMove.x=nInput;
    validMove.y=cInput;

    //If the user enters a new valid command other then SAVE FILE.TXT or LOAD FILE.TXT, it is assigned as the new move.
    return validMove;
}

//This function chooses the neighbour cell that will be visited to control the win condition according to the neighbour priority number.
neighbour NeighbourDetermine(const char (&boardArray)[12][12], const cell& currentCell, int priorityNumber)
{
    neighbour determinedNeighbour;

    switch(priorityNumber)
    {
    case 1:
        determinedNeighbour.c=boardArray[currentCell.x-1][currentCell.y+1];
        determinedNeighbour.x=currentCell.x-1;
        determinedNeighbour.y=currentCell.y+1;
        break;
    case 2:
        determinedNeighbour.c=boardArray[currentCell.x][currentCell.y+1];
        determinedNeighbour.x=currentCell.x;
        determinedNeighbour.y=currentCell.y+1;
        break;
    case 3:
        determinedNeighbour.c=boardArray[currentCell.x+1][currentCell.y];
        determinedNeighbour.x=currentCell.x+1;
        determinedNeighbour.y=currentCell.y;
        break;
    case 4:
        determinedNeighbour.c=boardArray[currentCell.x+1][currentCell.y-1];
        determinedNeighbour.x=currentCell.x+1;
        determinedNeighbour.y=currentCell.y-1;
        break;
    case 5:
        determinedNeighbour.c=boardArray[currentCell.x][currentCell.y-1];
        determinedNeighbour.x=currentCell.x;
        determinedNeighbour.y=currentCell.y-1;
        break;
    case 6:
        determinedNeighbour.c=boardArray[currentCell.x-1][currentCell.y];
        determinedNeighbour.x=currentCell.x-1;
        determinedNeighbour.y=currentCell.y;
    }

    return determinedNeighbour;
}

//This function checks if the cell was registered as a past cell while controlling the win condition.
int PastControl(int a, const cell (&pastCells)[144], const neighbour& cellNeighbour)
{
    int pastCheck=0;

    for(int i=0; i<a; ++i)
        if(pastCells[i].x==cellNeighbour.x && pastCells[i].y==cellNeighbour.y)
                pastCheck=1;

    return pastCheck;
}

//This function checks if the cell was registered as a removed cell while controlling the win condition.
int RemovedControl(int a, const cell (&removedCells)[144], const neighbour& cellNeighbour)
{
    int removedCheck=0;

    for(int i=0; i<a; ++i)
        if(removedCells[i].x==cellNeighbour.x && removedCells[i].y==cellNeighbour.y)
            removedCheck=1;

    return removedCheck;
}

//This function changes the chosen cell's coordinates with the registered neighbour cell's coordinates.
void NeighbourRegister(cell& chosenCell, const neighbour& registeredNeighbour, int& a, cell (&pastCells)[144], int& check)
{
    //Registers the registered neighbour as a past cell not to choose it again int the same loop. Can choose again in a spesific condition.
    pastCells[a].x=registeredNeighbour.x;
    pastCells[a].y=registeredNeighbour.y;
    a+=1;
    //Registers the registered neighbour as the new chosen cell.
    chosenCell.x=registeredNeighbour.x;
    chosenCell.y=registeredNeighbour.y;
    check+=1;
}

//This function checks if a specific player has won the game.
int WinCheck(char (&boardArray)[12][12], int boardSize, char PlayerControl)
{
    cell startCell;//Stores the starting cell's coordinates
    cell PastCell[144];
    cell RemovedCell[144];
    neighbour chosenNeighbour;
    int winPoint;//Stores which column or row we are at while choosing new start cells.
    int startCheck=0;
    int chosenCheck=0;//0 if no neighbour was chosen before, 1 if a neighbour was already choosen
    int wincheck=0;//0 if no win condition, 1 if there is a win condition
    int j=0;
    int k=0;
    int m=0;

    for(int i=0; i<boardSize; ++i)
    {
        //Checks if there is a starting cell to start checking if there is a path with X's or O's on the very right or very left according to which player we are checking for. Assigns it, if there is.
        if(boardArray[i][j]==PlayerControl && PlayerControl==static_cast<char>(CellState::User1orComputer))
        {
            startCell.x=i;
            startCell.y=j;
            startCheck=1;
        }
        if(boardArray[j][i]==PlayerControl && PlayerControl==static_cast<char>(CellState::User2))
        {
            startCell.x=j;
            startCell.y=i;
            startCheck=1;
        }

        if(startCheck==1)//If there is a starting cell, starts checking the neighbors in a spesific order.
        {
            auto startTemp=startCell;//Stores the very first starting cell's coordinates
            //Assigns the starting cell as a past and removed cell not to choose it again unles it is especially want to  be chosen again.
            PastCell[k]=startCell;
            k++;
            RemovedCell[m]=startCell;
            m++;

            while(winPoint!=(boardSize-1))
            {
                auto formerTemp=startCell;//Stores the former starting cell's coordinates

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp);//Checking and registering neighbour which has the first priority if it is not a past or removed cell and contains the PlayerControl.
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp, 2);//Checking and registering neighbour which has the second priority if it is not a past or removed cell and contains the PlayerControl. This goes on like that.
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp, 3);//Priority 3
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp, 4);//Priority 4
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp, 5);//Priority 5
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                chosenNeighbour=NeighbourDetermine(boardArray, formerTemp, 6);//Priority 6
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(startCell, chosenNeighbour, k, PastCell, chosenCheck);

                if(chosenCheck==0 && startCell.x==startTemp.x && startCell.y==startTemp.y)//If there is no more starting cells to choose from neighbors, exits the loop
                    break;

                if(chosenCheck==0)//If there is no more starting cells to choose from neighbors but can return to the very first starting cell, returns.
                {
                    //Removes the current starting cell not to choose in the same loop.
                    RemovedCell[m]=startCell;
                    m++;
                    k=0;//Clearing the past cell informations

                    startCell=startTemp;
                }

                //winPoint will be determined according to which side we want to reach on the board. It should be bottom or the very left side according to the which player we are checking for.
                if(PlayerControl==static_cast<char>(CellState::User1orComputer))
                    winPoint=startCell.y;
                else
                    winPoint=startCell.x;

                chosenCheck=0;
            }

            if(winPoint==(boardSize-1))//If it has been reached to the other side of the board while choosing new start cells, it means there is a win condition.
            {
                wincheck=1;//If wincheck is 1, it means there is a winner. That's why there is no need to check the other cells on the left or top, exits the loop.
                break;
            }
            k=0;//Clearing the past cell informations for the next possible loop
            m=0;//Clearing the removed cell informations for the next possible loop
        }
        startCheck=0;
    }
    return wincheck;
}
