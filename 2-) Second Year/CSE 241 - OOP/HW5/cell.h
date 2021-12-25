#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

using namespace std;

namespace HexGame_200104004094
{
    enum class CellState: char{//Holds the state of the cells
        Empty='.',
        User1orUser='X',
        User2orComputer='O',
    };

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

            int getNeighbourCellRow()const{return nRow;}
            char getNeighbourCellColumn()const{return nColumn;}

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

        int getCellRow()const{return row;}
        char getCellColumn()const{return column;}
        CellState getCellState()const{return state;}
        const NeighbourCell& getNeighbourCell(int neighbourNumber)const{return neighbourCells[neighbourNumber-1];}

        void updateCellState(char newState);

    private:
        NeighbourCell* neighbourCells=nullptr;//Size of it is always 6
        int row;
        char column;
        CellState state;
    };

    bool operator==(const Cell& current, const Cell& other);
}

#endif // CELL_H_INCLUDED
