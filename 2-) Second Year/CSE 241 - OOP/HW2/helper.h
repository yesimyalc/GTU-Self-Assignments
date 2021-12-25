#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <string>

using namespace std;

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
enum class CellState: char{//Holds the state of the cells
    Empty='.',
    User1orComputer='X',
    User2='O',
};

//This function displays the hex game's board.
void DisplayBoard(const char (&boardArray)[12][12], int boardSize);

//This function gets the user command for cell moves or for going to the menu.
void getCommand(string& inputCommand, int& nInput, char& cInput);

//This function goes to load&save menu and does operations accordingly
void menuOperations(const string& inputCommand, char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, char& backCommand, int& totalGameNumber);

//This function saves the game to SavedGames.txt file.
void saveFile(char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, int& totalGameNumber);

//This function loads a game from the SavedGames.txt file
void loadFile(char (&boardArray)[12][12], int& boardSize, char& currentMethod, string& currentTurn, chosenMove& currentMove, int& totalGameNumber);

//This function makes the player moves
void makeMove(char (&boardArray)[12][12], int boardSize, const string& currentTurn, const chosenMove& currentMove);

//This function checks if the move of a player was a legal move.
chosenMove ValidMoveCheck(const char (&boardArray)[12][12], int boardSize, int nInput, char cInput, chosenMove oldMove, string& inputCommand);

//This function chooses the neighbour cell that will be visited to control the win condition according to the neighbour priority number.
neighbour NeighbourDetermine(const char (&boardArray)[12][12], const cell& currentCell, int priorityNumber=1);

//This function checks if the cell was registered as a past cell while controlling the win condition.
int PastControl(int a, const cell (&pastCells)[144], const neighbour& cellNeighbour);

//This function checks if the cell was registered as a removed cell while controlling the win condition.
int RemovedControl(int a, const cell (&removedCells)[144], const neighbour& cellNeighbour);

//This function changes the chosen cell's coordinates with the registered neighbour cell's coordinates.
void NeighbourRegister(struct cell& chosenCell, const neighbour& registeredNeighbour, int& a, cell (&pastCells)[144], int& check);

//This function checks if a specific player has won the game.
int WinCheck(char (&boardArray)[12][12], int boardSize, char PlayerControl=static_cast<char>(CellState::User1orComputer));


#endif // HELPER_H

