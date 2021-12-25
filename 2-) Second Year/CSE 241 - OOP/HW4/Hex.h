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
    explicit Hex(string gameTypeInfo);//initializes as the given game type and min board sizes
    Hex();//Initializes as min board and computer vs player

    //Big Three Provided
    Hex(const Hex& other);
    ~Hex();
    const Hex& operator =(const Hex& other);

    friend ostream& operator<<(ostream& outputStream, const Hex& writtenHex);
    friend ifstream& operator>>(ifstream& inputStream, Hex& readGame);
    Hex operator--();
    Hex operator--(int ignore);
    bool operator==(const Hex& other)const;//If the current game has more occupied cells for whoever has the turn (user1 or user2), returns true.

    class Cell
    {
    public:
        Cell();//Initializes as empty A1 cell

        //Big Three provided
        Cell(const Cell& other);
        ~Cell();
        const Cell& operator=(const Cell& other);

        class NeighbourCell
        {
        public:
            NeighbourCell();//Initializes as A1

            //No need for custom Big Three

            void setNeighbourCell(int nRowNumber, char nColumnPoisiton);//Sets the neighbour cells as the given values

            inline int getNeighbourCellRow()const{return nRow;}
            inline char getNeighbourCellColumn()const{return nColumn;}

        private:
            int nRow;
            char nColumn;
        };

        friend ostream& operator<<(ostream& outputStream, const Cell& writtenCell);
        friend istream& operator>>(istream& inputStream, Cell& readCell);

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
        NeighbourCell* neighbourCells=nullptr;//Size of it is always 6
        int row;
        char column;
        CellState state;
    };

    inline int getBoardHeight()const{return height;}
    inline int getBoardWidth()const{return width;}
    inline string getGameType()const{return gameType;}
    inline string getWhoseTurn()const{return whoseTurn;}
    inline int getXCount()const{return xCount;}
    inline int getOCount()const{return oCount;}
    inline int getXScore()const{return xScore;}
    inline int getOScore()const{return oScore;}
    inline int getMoveCount()const{return moveCount;}
    int getUserScore()const;//Returns the current user's score

    void displayBoard()const;
    void setCommand();//Gets command from the user
    void gameMenu();//If MENU command is entered, goes here and gets input to switch to another game
    void saveGame();
    void loadGame();
    bool winCheck();//Returns true if there is a win condition

    Cell& play();//Plays the game for one move for a user
    Cell& play(Cell cMove);//Plays the game for one move for the computer
    void playGame();//Plays the game for the user1 or user then for the user2 or for the computer.

    inline static string getCommand(){return command;}
    static int getAllMarkedCells(){return allMarkedCells;}

    static int currentGameNo;//Stores the current game's game number
    static int gameCount;//Stores how many games are opened in total

private:

    Cell** hexCells=nullptr;//Size of it is determined according to height*width
    int height;
    int width;
    string gameType;//user vs user or user vs computer
    string whoseTurn;

    int xCount;
    int oCount;
    int xScore;
    int oScore;
    Cell chosenCell;//chosen cell as a move
    Cell* moves=nullptr;//Size of it is determined according to height*width
    int moveCount;

    void setHexGame();//Used in initializations. Checks for validity etc
    void setHexGame(int newHeightValue, int newWidthValue);//For future changes if necessary
    void setHexGameBoard();//Only checks the board sizes then sets it
    void controlCell(int& chosenRow, char& chosenColumn);//Checks if the cell input for move is valid
    bool neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, Cell* pastCells);
    //Used in winCheck. Checks if there is a path that leads to a win condition

    static string command;
    static int allMarkedCells;
};

#endif // HELPER_H

