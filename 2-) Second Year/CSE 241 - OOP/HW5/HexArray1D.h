#ifndef HEXARRAY1D_H_INCLUDED
#define HEXARRAY1D_H_INCLUDED

#include <string>
#include <vector>
#include "cell.h"
#include "AbstractHex.h"

using namespace std;

namespace HexGame_200104004094
{
    class HexArray1D : public AbstractHex
    {
    public:
        //Constructors
        HexArray1D();
        HexArray1D(int heightValue, int widthValue);
        HexArray1D(int heightValue, int widthValue, string gameTypeInfo);

        //Big Three
        HexArray1D(const HexArray1D& other);
        virtual ~HexArray1D();
        HexArray1D& operator=(const HexArray1D& other);

        //Overloaded Operators
        friend ostream& operator<<(ostream& outputStream, const HexArray1D& current);
        virtual bool operator==(const AbstractHex& other)const;//Returns true if the hex games' boards' are equal
        virtual char operator()(int indexi, int indexj)const;//Returns the given cell content, Throws InvalidBoardCellException

        //Setters
        virtual void setBoard();//Initializes the board
        virtual void setCommand();

        //Other Functions
        virtual void reset();//Resets the board to the beginning
        virtual void print()const;//Prints the game board
        virtual void readFromFile(string file);//Throws CannotOpenFileException
        virtual void writeToFile(string file)const;//Throws CannotOpenFileException
        virtual void controlCell(int chosenRow, char chosenColum);//Checks if the cell input for move is valid
        virtual const Cell& play();//Plays one step for the computer
        virtual const Cell& play(Cell cMove);//Plays one setp for the user
        virtual void playGame();//Starts playing the game
        virtual bool isEnd()const;//Checks if there is a win condition

    private:
        Cell* hexCells=nullptr;

        bool neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, vector<Cell> &pastCells)const;
        void allocateMemory();//Throws MemoryAllocationException
    };
}
#endif // HEXARRAY1D_H_INCLUDED
