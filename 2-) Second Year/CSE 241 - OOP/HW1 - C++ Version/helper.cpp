#include <iostream>
#include "helper.h"

using namespace std;

void DisplayBoard(char boardArray[12][12], int boardSize)
{
    int i;
    int j;
    int l;

    for(i=0; i<boardSize; ++i)
      for(j=0; j<boardSize; ++j)
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

chosenMove ValidMoveCheck(char boardArray[12][12], int boardSize, int nInput, char cInput)
{
    /*Checks if the move that was done by the player is correct by the validCheck.
    If any the check is correct, it will become 1, if not it will stay as 0. Until it becomes 1, player is forced to make a new move.
    When the cell is a valid one, the cell coordinates are returned in a structure.*/

    int validCheck=0;
    chosenMove validMove;//This will store the last determined valid move.

    if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]=='.'))
        validCheck=1;

       while(validCheck==0)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(100,'\n');
        }
        cout<<"Move you wanted to do was not legal, please make another move."<<endl<<
        "Choose your move: "<<endl;
        cin>>cInput>>nInput;

        if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]=='.'))
           validCheck=1;
    }

    validMove.x=nInput;
    validMove.y=cInput;

    cin.clear();
    cin.ignore(256,'\n');

    return validMove;
}

neighbour NeighbourDetermine(int neighbourNumber, char boardArray[12][12], cell currentCell)
{
    neighbour determinedNeighbour;

    switch(neighbourNumber)
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

int PastControl(int a, cell pastCells[], neighbour cellNeighbour)
{
    int i;
    int pastCheck=0;

    for(i=0; i<a; ++i)
        if(pastCells[i].x==cellNeighbour.x && pastCells[i].y==cellNeighbour.y)
                pastCheck=1;

    return pastCheck;
}

int RemovedControl(int a, cell removedCells[], neighbour cellNeighbour)
{
    int i;
    int removedCheck=0;

    for(i=0; i<a; ++i)
        if(removedCells[i].x==cellNeighbour.x && removedCells[i].y==cellNeighbour.y)
            removedCheck=1;

    return removedCheck;
}

void NeighbourRegister(cell* chosenCell, neighbour registeredNeighbour, int *a, cell* pastCells, int *check)
{
    pastCells[*a].x=registeredNeighbour.x;
    pastCells[*a].y=registeredNeighbour.y;
    *a+=1;
    chosenCell->x=registeredNeighbour.x;
    chosenCell->y=registeredNeighbour.y;
    *check+=1;
}

int WinCheck(char boardArray[12][12], int boardSize, char PlayerControl)
{
    /*If the PlayerControl is 'X', searches for any cells that contains 'X' on the very left side. If there is, it starts checking the cell's neighbors in a specific order.
    When it finds a neighbour cell that contains 'X' inside, registers that cell as the new starting cell also registers the former cell as a past cell and continues while avoiding past cells.
    When there are no neighbors that contains 'X' inside, stores the latest start cell as a removed cell not to register it again.
    Starts from the beginning while avoiding the past removed cells.
    It continues till it finds a cell that contains 'X' on the very right side (win condition), or there are no available start cells to choose.
    If the PlayerControl is 'O' does the same thing by starting from the top, trying to reach to bottom.*/

    cell startCell;//Stores the starting cell's coordinates
    cell startTemp;//Stores the very first starting cell's coordinates
    cell formerTemp;//Stores the former starting cell's coordinates
    cell PastCell[144];
    cell RemovedCell[144];
    neighbour chosenNeighbour;
    int winPoint;
    int startCheck=0;
    int chosenCheck=0;//0 if no neighbour was chosen before, 1 if a neighbour was already choosen
    int wincheck=0;//0 if no win condition, 1 if there is a win condition
    int i;
    int j=0;
    int k=0;
    int m=0;

    for(i=0; i<boardSize; ++i)
    {
        if(boardArray[i][j]==PlayerControl && PlayerControl=='X')//Checks if there is a starting cell.
        {
            startCell.x=i;
            startCell.y=j;
            startCheck=1;
        }
        if(boardArray[j][i]==PlayerControl && PlayerControl=='O')
        {
            startCell.x=j;
            startCell.y=i;
            startCheck=1;
        }

        if(startCheck==1)//If there is a starting cell, starts checking the neighbors.
        {
            startTemp.x=startCell.x;
            startTemp.y=startCell.y;
            PastCell[k].x=startCell.x;
            PastCell[k].y=startCell.y;
            k++;
            RemovedCell[m].x=startCell.x;
            RemovedCell[m].y=startCell.y;
            m++;

            while(winPoint!=(boardSize-1))
            {
                formerTemp.x=startCell.x;
                formerTemp.y=startCell.y;

                chosenNeighbour=NeighbourDetermine(1, boardArray, formerTemp);//Checking and registering neighbour 1 if it is not a past or removed cell and contains the PlayerControl.
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                chosenNeighbour=NeighbourDetermine(2, boardArray, formerTemp);//Neighbour 2
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                chosenNeighbour=NeighbourDetermine(3, boardArray, formerTemp);//Neighbour 3
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                chosenNeighbour=NeighbourDetermine(4, boardArray, formerTemp);//Neighbour 4
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                chosenNeighbour=NeighbourDetermine(5, boardArray, formerTemp);//Neighbour 5
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                chosenNeighbour=NeighbourDetermine(6, boardArray, formerTemp);//Neighbour 6
                if(chosenNeighbour.c==PlayerControl && chosenCheck==0 && PastControl(k, PastCell, chosenNeighbour)==0 && RemovedControl(m, RemovedCell, chosenNeighbour)==0)
                    NeighbourRegister(&startCell, chosenNeighbour, &k, PastCell, &chosenCheck);

                if(chosenCheck==0 && startCell.x==startTemp.x && startCell.y==startTemp.y)//If there is no more starting cells to choose from neighbors, exits the loop
                    break;

                if(chosenCheck==0)//If there is no more starting cells to choose from neighbors but can return to the very first starting cell, returns.
                {
                    RemovedCell[m].x=startCell.x;
                    RemovedCell[m].y=startCell.y;
                    m++;
                    k=0;
                    startCell.x=startTemp.x;
                    startCell.y=startTemp.y;
                }

                if(PlayerControl=='X')
                    winPoint=startCell.y;
                else
                    winPoint=startCell.x;

                chosenCheck=0;
            }

            if(winPoint==(boardSize-1))//If wincheck is 1, it means there is a winner. That's why there is no need to check the other cells on the left or top, exits the loop.
            {
                wincheck=1;
                break;
            }
            k=0;
            m=0;
        }
        startCheck=0;
    }
    return wincheck;
}
