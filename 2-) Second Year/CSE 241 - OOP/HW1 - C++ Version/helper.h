#ifndef HELPER_H
#define HELPER_H

struct chosenMove//This structure will store cell coordinates.
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

//This function displays the hex game's board.
void DisplayBoard(char boardArray[12][12], int boardSize);

//This function checks if the move of a player was a legal move.
chosenMove ValidMoveCheck(char boardArray[12][12], int boardSize, int nInput, char cInput);

//Chooses the neighbour cell that will be visited to control the win condition.
neighbour NeighbourDetermine(int neighbourNumber, char boardArray[12][12], cell currentCell);

//Checks if the cell was registered as a past cell while controlling the win condition.
int PastControl(int a, cell pastCells[], neighbour cellNeighbour);

//Checks if the cell was registered as a removed cell while controlling the win condition.
int RemovedControl(int a, cell removedCells[], neighbour cellNeighbour);

//Changes the chosen cell's coordinates with the registered neighbour cell's coordinates.
void NeighbourRegister(struct cell* chosenCell, neighbour registeredNeighbour, int *a, cell* pastCells, int *check);

//This function checks if a specific player has won the game.
int WinCheck(char boardArray[12][12], int boardSize, char PlayerControl);

#endif // HELPER_H

