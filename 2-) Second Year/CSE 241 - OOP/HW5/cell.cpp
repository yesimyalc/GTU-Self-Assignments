#include <iostream>
#include "cell.h"

namespace HexGame_200104004094
{
    Cell::NeighbourCell::NeighbourCell()  : nRow(1), nColumn('A')
    {/*Intenionally left empty*/}

    void Cell::NeighbourCell::setNeighbourCell(int nRowNumber, char nColumnPosition)
    {
        nRow=nRowNumber;
        nColumn=nColumnPosition;
    }

    Cell::Cell()
    {
        setCell(0, 'A', CellState::Empty);
    }

    //Big Three
    Cell::Cell(const Cell& other)
    {
        setCell(other.getCellRow(), other.getCellColumn(), other.getCellState());
    }

    Cell::~Cell()
    {
        delete[] neighbourCells;
    }

    const Cell& Cell::operator=(const Cell& other)
    {
        if(this==&other)
            return *this;
        else
        {
            setCell(other.getCellRow(), other.getCellColumn(), other.getCellState());
            return *this;
        }
    }

    void Cell::setCell(int rowNumber, char columnPosition, CellState stateInfo)
    {
        if(neighbourCells==nullptr)
        {
            neighbourCells=new NeighbourCell[6];
            if(neighbourCells==nullptr)
            {
                cout<<"Cannot allocate memory."<<endl;
                exit(-1);
            }
        }
        row=rowNumber;
        column=columnPosition;
        state=stateInfo;
        setNeighbourCells();
    }

    //Sets only the row and the column numbers of a cell. This will be used to set cells which we dont care about their states and neighbours
    void Cell::setCell(int rowNumber, char columnPosition)
    {
        row=rowNumber;
        column=columnPosition;
    }

    //Sets the cell's 6 neighbours. There might be neighbours outside of the game's board too but they will be ignored later on
    void Cell::setNeighbourCells()
    {
        for(int i=0; i<6; ++i)
        {
            switch(i+1)
            {
            case 1:
                neighbourCells[i].setNeighbourCell((getCellRow()-1), (getCellColumn()+1));
                break;
            case 2:
                neighbourCells[i].setNeighbourCell(getCellRow(), (getCellColumn()+1));
                break;
            case 3:
                neighbourCells[i].setNeighbourCell((getCellRow()+1), getCellColumn());
                break;
            case 4:
                neighbourCells[i].setNeighbourCell((getCellRow()+1), (getCellColumn()-1));
                break;
            case 5:
                neighbourCells[i].setNeighbourCell(getCellRow(), (getCellColumn()-1));
                break;
            case 6:
                neighbourCells[i].setNeighbourCell((getCellRow()-1), getCellColumn());
            }
        }
    }

    //Updates the cell's state when that cell was chosen as a move or in a load game condition etc
    void Cell::updateCellState(char newState)
    {
        if(newState=='X')
            state=CellState::User1orUser;
        else if(newState=='O')
            state=CellState::User2orComputer;
        else
            state=CellState::Empty;
    }

    //Only prints out the cell's position and the state because other informations about the cells can stay hidden, they are not useful to print out
    ostream& operator<<(ostream& outputStream, const Cell& writtenCell)
    {
        outputStream<<endl<<writtenCell.getCellColumn()<<writtenCell.getCellRow()<<"\t"<<static_cast<char>(writtenCell.getCellState());
        return outputStream;
    }

    istream& operator>>(istream& inputStream, Cell& readCell)
    {
        char temp;
        inputStream>>readCell.column>>readCell.row>>temp;
        readCell.updateCellState(temp);
        readCell.setNeighbourCells();

        return inputStream;
    }

    bool operator==(const Cell& current, const Cell& other)
    {
        if(current.getCellRow()==other.getCellRow() && current.getCellColumn()==other.getCellColumn() && current.getCellState()==other.getCellState())
            return true;
        else
            return false;
    }
}
