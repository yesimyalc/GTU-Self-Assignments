#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>

using namespace std;

enum class CellState: char{//Holds the state of the cells
    Empty='.',
    User1orUser='X',
    User2orComputer='O',
};

class Hex
{
public:
    Hex(int heightValue, int widthValue, string gameTypeInfo);//Initializes as the given values.
    Hex(string gameTypeInfo);//initializes as the given game type and min board sizes
    Hex();//Initializes as min board and computer vs player

    inline int getBoardHeight()const{return height;}
    inline int getBoardWidth()const{return width;}
    inline string getGameType()const{return gameType;}
    inline string getWhoseTurn()const{return whoseTurn;}
    inline int getXCount()const{return xCount;}
    inline int getOCount()const{return oCount;}

    void displayBoard()const;
    void setCommand();//Gets command from the user
    void gameMenu();//If MENU command is entered, goes here and gets input to switch to another game
    void saveGame()const;
    void loadGame();
    bool winCheck()const;//Returns true if there is a win condition
    bool compareGames(const Hex& otherGame)const;
    /*If the current game has more occupied cells for whoever has the turn (user1 or user2), the first game wins over the other game.*/
    void play();//Plays the game for one move for a user
    void play(int chosenRow, char chosenColumn);//Plays the game for one move for the computer
    void playGame();//Plays the game for the user1 or user then for the user2 or for the computer.

    inline static string getCommand(){return command;}
    static int getAllMarkedCells(){return allMarkedCells;}

    static int currentGameNo;//Stores the current game's game number
    static int gameCount;//Stores how many games are opened in total

private:
    class Cell
    {
    public:
        Cell();

        class NeighbourCell
        {
        public:
            NeighbourCell();

            void setNeighbourCell(int nRowNumber, char nColumnPoisiton);//Sets the neighbour cells as the given values

            inline int getNeighbourCellRow()const{return nRow;}
            inline char getNeighbourCellColumn()const{return nColumn;}

        private:
            int nRow;
            char nColumn;
        };

        void setCell(int rowNumber, char columnPosition, CellState stateInfo);//Sets the cell as the given values
        void setCell(int rowNumber, char columnPosition);
        //Sets only the row and the column numbers of a cell. This will be used to set cells which we dont care about their states and neighbours
        void setNeighbourCells();//Sets the cell's neighbours

        inline int getCellRow()const{return row;}
        inline char getCellColumn()const{return column;}
        inline CellState getCellState()const{return state;}
        inline const NeighbourCell& getNeighbourCell(int neighbourNumber)const{return neighbourCells[neighbourNumber-1];}

        void updateCellState(char newState);

    private:
        vector<NeighbourCell> neighbourCells;
        int row;
        char column;
        CellState state;
    };

    vector< vector<Cell> > hexCells;
    int height;
    int width;
    string gameType;//user vs user or user vs computer
    string whoseTurn;

    int xCount;
    int oCount;
    Cell lastChosenCell;//previously chosen cell as a move
    Cell chosenCell;//chosen cell as a move

    void setHexGame();//Used in initializations. Checks for validity, resizes vectors etc
    void setHexGame(int newHeightValue, int newWidthValue);//For future changes if necessary
    void setHexGameBoard();//Only checks the board sizes then sets it
    void controlCell(int& chosenRow, char& chosenColumn);//Checks if the cell input for move is valid
    bool neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, vector<Cell> &pastCells)const;
    //Used in winCheck. Checks if there is a path that leads to a win condition

    static string command;
    static vector<int> fullSavedGameSlots;//Stores how many games were saved before
    static int allMarkedCells;

    inline static vector<int> getFullSavedGameSlots(){return fullSavedGameSlots;}
    static void getSavedGames();
};

#endif // HELPER_H

