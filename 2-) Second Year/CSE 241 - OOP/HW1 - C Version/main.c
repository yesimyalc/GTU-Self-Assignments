#include <stdio.h>
#include <stdlib.h>

struct move//This structure will store cell coordinates.
{
    int x;
    char y;
};

struct cell//This structure will store cell coordinates.
{
    int x;
    int y;
};

struct neighbour//This structure will store neighbour cells' coordinates and contents.
{
    char c;
    int x;
    int y;
};

void DisplayBoard(char boardArray[12][12], int boardSize)//This function displays the hex game's board.
{
    int i;
    int j;
    int l;

    for(i=0; i<boardSize; ++i)
      for(j=0; j<boardSize; ++j)
      {
        if(i==0 && j==0)
        {
            printf("  ");
            for(l=0; l<boardSize; l++)
              printf("%c ", 'A'+l);
        }

        if(j==0)
        {
            printf("\n%d", i+1);
              for(l=0; l<i+1; ++l)
                printf(" ");
        }
        printf("%c ", boardArray[i][j]);
       }
}

struct move ValidMoveCheck(char boardArray[12][12], int boardSize, int number, char character)//This function checks if the move of a player was a legal move.
{
    /*Checks if the move that was done by the player is correct by the validCheck.
    If any the check is correct, it will become 1, if not it will stay as 0. Until it becomes 1, player is forced to make a new move.
    When the cell is a valid one, the cell coordinates are returned in a structure.*/

    int nInput=number;//This will store the info of the x coordinate of the cell that is choosen by a player.
    char cInput=character;//This will store the info of the y coordinate of the cell that is choosen by a player.
    int validCheck=0;
    struct move Move;//This will store the last determined valid move.

    if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]=='.'))
        validCheck=1;

       while(validCheck==0)
    {
        printf("Move you wanted to do was not legal, please make another move.\n");
        printf("Choose your move: \n");
        scanf(" %c%d", &cInput, &nInput);

        if((nInput>=0) && (nInput<=boardSize) && (((int)cInput)-((int)'A')>=0) && (((int)cInput)-((int)'A')<=boardSize) && (boardArray[nInput-1][((int)cInput)-((int)'A')]=='.'))
           validCheck=1;
    }

    Move.x=nInput;
    Move.y=cInput;

    return Move;
}

int DrawCheck(char boardArray[12][12], int boardSize)//This function checks if there is a draw.
{
    //Searchs for any '.' in the board. If there is any '.', it means there are still available moves so there is no draw condition.
    int i;
    int j;
    int drawCheck=0;

    for(i=0; i<boardSize; i++)
        for(j=0; j<boardSize; ++j)
            if(boardArray[i][j]=='.')
            {
                drawCheck=1;
                break;
            }

    return drawCheck;//Returns 1 if there is not a draw, returns 0 if there is.
}

struct neighbour NeighbourDetermine(int neighbourNumber, char boardArray[12][12], struct cell formerCell)//Chooses the neighbour cell that will be visited to control the win condition.
{
    struct neighbour cellNeighbour;

    switch(neighbourNumber)
    {
    case 1:
        cellNeighbour.c=boardArray[formerCell.x-1][formerCell.y+1];
        cellNeighbour.x=formerCell.x-1;
        cellNeighbour.y=formerCell.y+1;
        break;
    case 2:
        cellNeighbour.c=boardArray[formerCell.x][formerCell.y+1];
        cellNeighbour.x=formerCell.x;
        cellNeighbour.y=formerCell.y+1;
        break;
    case 3:
        cellNeighbour.c=boardArray[formerCell.x+1][formerCell.y];
        cellNeighbour.x=formerCell.x+1;
        cellNeighbour.y=formerCell.y;
        break;
    case 4:
        cellNeighbour.c=boardArray[formerCell.x+1][formerCell.y-1];
        cellNeighbour.x=formerCell.x+1;
        cellNeighbour.y=formerCell.y-1;
        break;
    case 5:
        cellNeighbour.c=boardArray[formerCell.x][formerCell.y-1];
        cellNeighbour.x=formerCell.x;
        cellNeighbour.y=formerCell.y-1;
        break;
    case 6:
        cellNeighbour.c=boardArray[formerCell.x-1][formerCell.y];
        cellNeighbour.x=formerCell.x-1;
        cellNeighbour.y=formerCell.y;
    }

    return cellNeighbour;
}

int PastControl(int a, struct cell pastCells[a], struct neighbour cellNeighbour)//Checks if the cell was registered as a past cell while controlling the win condition.
{
    int i;
    int pastCheck=0;

    for(i=0; i<a; ++i)
        if(pastCells[i].x==cellNeighbour.x && pastCells[i].y==cellNeighbour.y)
                pastCheck=1;

    return pastCheck;
}

int RemovedControl(int a, struct cell removedCells[a], struct neighbour cellNeighbour)//Checks if the cell was registered as a removed cell while controlling the win condition.
{
    int i;
    int removedCheck=0;

    for(i=0; i<a; ++i)
        if(removedCells[i].x==cellNeighbour.x && removedCells[i].y==cellNeighbour.y)
            removedCheck=1;

    return removedCheck;
}

void NeighbourRegister(struct cell* chosenCell, struct neighbour cellNeighbour, int *a, struct cell* pastCells, int *check)//Changes the starting cell's coordinates with the chosen neighbour cell's coordinates.
{
    pastCells[*a].x=cellNeighbour.x;
    pastCells[*a].y=cellNeighbour.y;
    *a+=1;
    chosenCell->x=cellNeighbour.x;
    chosenCell->y=cellNeighbour.y;
    *check+=1;
}

int WinCheck(char boardArray[12][12], int boardSize, char PlayerControl)//This function checks if a specific player has won the game.
{
    /*If the PlayerControl is 'X', searches for any cells that contains 'X' on the very left side. If there is, it starts checking the cell's neighbors in a specific order.
    When it finds a neighbour cell that contains 'X' inside, registers that cell as the new starting cell also registers the former cell as a past cell and continues while avoiding past cells.
    When there are no neighbors that contains 'X' inside, stores the latest start cell as a removed cell not to register it again.
    Starts from the beginning while avoiding the past removed cells.
    It continues till it finds a cell that contains 'X' on the very right side (win condition), or there are no available start cells to choose.
    If the PlayerControl is 'O' does the same thing by starting from the top, trying to reach to bottom.*/

    struct cell startCell;//Stores the starting cell's coordinates
    struct cell startTemp;//Stores the very first starting cell's coordinates
    struct cell formerTemp;//Stores the former starting cell's coordinates
    struct cell PastCell[144];
    struct cell RemovedCell[144];
    struct neighbour chosenNeighbour;
    int winPoint;
    int startCheck=0;
    int chosenCheck=0;//0 if no neighbour was chosen before, 1 if a neighbour was already choosen
    int wincheck=0;//0 if no win condition, 1 if there is a win condition
    int i;
    int j=0;
    int l;
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

int main()
{
    int boardSize;//This will store the hex game's board's size.
    char GameMethod;//This will store the info that determines if the game will be played with two users or with the computer.
    int numberInput;//This will store the info of the x coordinate of the cell that is choosen by a player or computer.
    int numberInput2;//This will store the info of the y coordinate of the cell that is choosen by the computer.
    char charInput;//This will store the info of the y coordinate of the cell that is choosen by a player.
    struct move myMove;//This will store the coordinates of a cell that a player chose as a move.
    int wincheck=0;//This will determine the win status. If it is equal to 0, it means no one has won yet.

    printf("Welcome to the Hex Game.\nPlease enter your desired board size.\n");
    printf("The board size should be between 6 and 12.(The size you enter will create a board which is (size)x(size).)\n");
    scanf("%d", &boardSize);
    while(boardSize<6 || boardSize>12)//Checking the board size info that we get from the user. If it is not a valid board size, it will repeat the process.
    {
        printf("You have entered an invalid size. Please enter again.\n");
        scanf("%d", &boardSize);
    }
    char board[12][12];//This will be the hex game's board's array. It determines each cell's coordinates and stores their contents.
    int i;
    int j;
    for(i=0; i<boardSize; ++i)
        for(j=0; j<boardSize; ++j)
            board[i][j]='.';

    printf("\nDo you want to play with two players(p) or play with the computer(c)? (p/c)\n");
    scanf(" %c", &GameMethod);
    while(GameMethod!='p' && GameMethod!='c')//Checking if the game method is a valid one.(u=two users/c=with computer)
    {
        printf("You have entered an invalid game method. Please enter again.\n");
        scanf(" %c", &GameMethod);
    }

    if(GameMethod=='p')
    {
        printf("\nThe match begins.\nPlayer 1 is using 'X' and Player 2 is using 'O'.\n");
        printf("Player 1 needs to connect very left and very right sides, player 2 needs to connect top and bottom.\n");
        printf("You can determine your move by entering dot coordinates. (example: F1, B1, A5)\n");
        printf("You can move to east, west, northeast, northwest, southeast or southwest.\n");

        DisplayBoard(board, boardSize);

        while(wincheck==0)
        {
            printf("\nPlayer 1's Turn\n");

            if(DrawCheck(board, boardSize)==0)//If there is a draw, exits the loop and the game ends.
            {
                printf("There is no other move options left. Because of that game ends with draw.\n");
                break;
            }

            printf("Choose your move: \n");
            scanf(" %c", &charInput);
            scanf("%d", &numberInput);
            myMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(myMove.x)-1][((int)myMove.y)-((int)'A')]='X';//Changing the cell's content to 'X', making it non available for another move.
            printf("The move was %c%d.\n", myMove.y, myMove.x);
            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'X')==1)
            {
                printf("\nPlayer 1 has won.\n");
                break;
            }

            printf("\nPlayer 2's Turn\n");

            if(DrawCheck(board, boardSize)==0)//If there is a draw, exits the loop and the game ends.
            {
                printf("There is no other move options left. Because of that game ends with draw.\n");
                break;
            }

            printf("Choose your move: \n");
            scanf(" %c", &charInput);
            scanf("%d", &numberInput);
            myMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(myMove.x)-1][((int)myMove.y)-((int)'A')]='O';//Changing the cell's content to 'O', making it non available for another move.
            printf("The move was %c%d.\n", myMove.y, myMove.x);
            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'O')==1)//If there is a win condition, exits the loop and the game ends.
            {
                printf("\nPlayer 2 has won.\n");
                break;
            }
        }
    }

    if(GameMethod=='c')
    {
        int startCheck=0;//Counts the times player has made a move

        printf("\nThe match begins.\nComputer is using 'X' and Player is using 'O'.\n");
        printf("Computer needs to connect very left and very right sides, player needs to connect top and bottom\n");
        printf("You can determine your move by entering dot coordinates. (example: F1, B1, A5)\n");
        printf("You can move to east, west, northeast, northwest, southeast or southwest.\n");

        DisplayBoard(board, boardSize);

        while(wincheck==0)
        {
            printf("\nComputer's Turn\n");

            if(DrawCheck(board, boardSize)==0)//If there is a draw, exits the loop and the game ends.
            {
                printf("There is no other move options left. Because of that game ends with draw.\n");
                break;
            }

            /*Computer AI starts here. The main logic of it is, choosing cells that are neighbours to the cell that computer's opponent has recently chosen.
            The computers priority is choosing cells that are top or bottom neighbours because the opponent is trying to connect up side to down side.
            By doing like that, the computer tries to block the opponent's paths.*/

            if(startCheck==0)//Because the player has never made a move before, computer makes it's first move according to the board boardSize
            {
                numberInput=boardSize/2;
                numberInput2=0;
                board[numberInput][numberInput2]='X';
            }

            if(startCheck!=0)//Makes a move according to the player's former move.
            {
                if(board[numberInput-1][numberInput2]=='.')
                {
                    board[numberInput-1][numberInput2]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+65), numberInput);
                }
                else if(board[numberInput+1][numberInput2]=='.')
                {
                    board[numberInput+1][numberInput2]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+65), numberInput+2);
                }
                else if(board[numberInput-1][numberInput2+1]=='.')
                {
                    board[numberInput-1][numberInput2+1]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+66), numberInput);
                }
                else if(board[numberInput+1][numberInput2-1]=='.')
                {
                    board[numberInput+1][numberInput2-1]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+64), numberInput+2);
                }
                else if(board[numberInput][numberInput2+1]=='.')
                {
                    board[numberInput][numberInput2+1]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+66), numberInput+1);
                }
                else if(board[numberInput][numberInput2-1]=='.')
                {
                    board[numberInput][numberInput2-1]='X';
                    printf("The move was %c%d.\n", (char)(numberInput2+64), numberInput);
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
                            printf("The move was %c%d.\n", (char)(numberInput2+65), numberInput+1);
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
                printf("\nComputer has won.\n");
                break;
            }

            printf("\nPlayer's Turn\n");

            if(DrawCheck(board, boardSize)==0)//If there is a draw, exits the loop and the game ends.
            {
                printf("There is no other move options left. Because of that game ends with draw.\n");
                break;
            }

            printf("Choose your move: \n");
            scanf(" %c", &charInput);
            scanf("%d", &numberInput);
            myMove=ValidMoveCheck(board, boardSize, numberInput, charInput);//Checking the validity of the move and storing the valid move.

            board[(myMove.x)-1][((int)myMove.y)-((int)'A')]='O';//Changing the cell's content to 'O', making it non available for another move.
            printf("The move was %c%d.\n", myMove.y, myMove.x);

            numberInput=(myMove.x)-1;
            numberInput2=((int)myMove.y)-((int)'A');

            startCheck++;

            DisplayBoard(board, boardSize);

            if(WinCheck(board, boardSize, 'O')==1)//If there is a win condition, exits the loop and the game ends.
            {
                printf("\nPlayer has won.\n");
                break;
            }
        }
    }
    return 0;
}
