#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include "cell.h"
#include "exceptions.h"
#include "AbstractHex.h"
#include "HexArray1D.h"

using namespace std;

namespace HexGame_200104004094
{
    HexArray1D::HexArray1D()
        :HexArray1D(6, 6, "user vs computer")
    {/*Intentionally left empty*/}

    HexArray1D::HexArray1D(int heightValue, int widthValue)
        :HexArray1D(heightValue, widthValue, "user vs computer")
    {/*Intentionally left empty*/}

    HexArray1D::HexArray1D(int heightValue, int widthValue, string gameTypeInfo)
        :AbstractHex(heightValue, widthValue, gameTypeInfo)
    {
        try
        {
            setBoard();
        }
        catch(MemoryAllocationException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    HexArray1D::HexArray1D(const HexArray1D& other)
    {
        *this=other;
    }

    HexArray1D::~HexArray1D()
    {
        //Calls the destructors
        for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
            hexCells[i].~Cell();
        //Releases the memory
        free (hexCells);
    }

    HexArray1D& HexArray1D::operator=(const HexArray1D& other)
    {
        if(this==&other)
            return *this;

        if(hexCells==nullptr || getBoardHeight()!=other.getBoardHeight())
        {
            try
            {
                //Release the memory and destruct the objects if necessary
                if(hexCells!=nullptr)
                {
                    for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
                        hexCells[i].~Cell();
                    free (hexCells);
                }

                height=other.getBoardHeight();
                width=other.getBoardWidth();

                //Allocate memory from the beginning
                allocateMemory();
            }
            catch(MemoryAllocationException& exc)
            {
                cerr<<exc.what()<<endl;
                exit(-1);
            }
        }

        //Filling the board
        for(int i=0; i<getBoardHeight(); ++i)
            for(int j=0; j<getBoardWidth(); ++j)
                hexCells[i*getBoardWidth()+j]=other.hexCells[i*getBoardWidth()+j];

        gameType=other.getGameType();
        whoseTurn=other.getWhoseTurn();
        gameCount=other.numberOfMoves();
        moves=other.moves;

        return *this;
    }

    ostream& operator<<(ostream& outputStream, const HexArray1D& current)
    {
        int l;

        for(int i=0; i<current.getBoardHeight(); ++i)
          for(int j=0; j<current.getBoardWidth(); ++j)
          {
            if(i==0 && j==0)
            {
                outputStream<<"  ";
                for(l=0; l<current.getBoardWidth(); l++)
                  outputStream<<" "<<static_cast<char>('A'+l);
            }

            if(j==0)
            {
                outputStream<<endl<<i+1;
                  for(l=0; l<i+1; ++l)
                    outputStream<<" ";
            }
            outputStream<<" "<<current(i,j);
           }
        outputStream<<endl;

        return outputStream;
    }

    //Returns true if the hex games' board' are the same. Does not care about other members etc
    bool HexArray1D::operator==(const AbstractHex& other)const
    {
        try
        {
            if(this==&other)
                return true;

            if(getBoardHeight()!=other.getBoardHeight())
                return false;

            for(int i=0; i<getBoardHeight(); i++)
                for(int j=0; j<getBoardWidth(); ++j)
                {
                        if(operator()(i, j)!=other(i, j))
                            return false;
                }
        }
        catch(InvalidBoardCellException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }

        return true;
    }

    //Returns the content of the given cell
    //Throws InvalidBoardException if there is no cell with the given coordinates
    char HexArray1D::operator()(int indexi, int indexj)const
    {
        if(indexi<0 || indexi>getBoardHeight() || indexj<0 || indexj>getBoardWidth())
            throw InvalidBoardCellException();

        return static_cast<char>(hexCells[getBoardWidth()*indexi+indexj].getCellState());
    }

    //Allocates memory for the board array
    //If it cannot allocate memory, throws MemoryAllocationException
    void HexArray1D::allocateMemory()
    {
        //Allocating the memory
        hexCells=(Cell*)malloc( sizeof(Cell)*(getBoardHeight()*getBoardWidth()) );
        if(hexCells==NULL)
            throw MemoryAllocationException();

        //Calling the necessary constructors
        for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
            new (hexCells+i) Cell();
    }

    //Initializes the board
    void HexArray1D::setBoard()
    {
        try
        {
            if(hexCells==nullptr)
                allocateMemory();

            for(int i=0; i<getBoardHeight(); ++i)
                for(int j=0; j<getBoardWidth(); ++j)
                    hexCells[i*getBoardWidth()+j].setCell(i+1, 'A'+j, CellState::Empty);
        }
        catch(MemoryAllocationException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    //Resets the board to the beginning
    void HexArray1D::reset()
    {
        AbstractHex::reset();
        //After resetting the members, initializes the board from the beginning
        setBoard();
    }

    //Prints the game board
    void HexArray1D::print()const
    {
        cout<<*this;
    }

    //Sets the commands for the player moves
    void HexArray1D::setCommand()
    {
        try
        {
            print();
            cout<<endl<<whoseTurn<<"'s Turn"<<endl;
            cout<<"Make your move: "<<endl;
            getline(cin, command);

            if(getCommand()=="SAVE")
            {
                cout<<endl<<"**********Save Game Menu**********"<<endl;

                cout<<"You can type the saved game file name you want and and save a game onto it."<<endl<<
                "If you write an already existed saved game file name, the game will be overwritten."<<endl<<
                "The saved game file name format should be like this: name.txt or name.TXT, name can be anything you want."<<endl;
                string fileName;
                getline(cin, fileName);

                string temp;
                while(fileName.size()<4)
                {
                    cout<<"You have entered an invalid file name type. Please enter again."<<endl;
                    getline(cin, fileName);
                }
                temp=fileName.substr((fileName.size()-4), 4);

                //If the given filename does not have .txt or .TXT inside, it will not be valid
                while(temp!=".txt" && temp!=".TXT")
                {
                    cout<<"You have entered an invalid file name type. Please enter again."<<endl;
                    getline(cin, fileName);

                    if(fileName.size()>3)
                        temp=fileName.substr((fileName.size()-4), 4);
                }

                //Start writing to the given file
                writeToFile(fileName);
                setCommand();
            }
            else if(getCommand()=="LOAD")
            {
                cout<<endl<<"**********Load Game Menu**********"<<endl;

                cout<<"Enter the saved game file you want to open. Do not forget to include .txt or .TXT in the file name."<<endl;
                string fileName;
                getline(cin, fileName);

                readFromFile(fileName);
                setCommand();
            }
            else if(getCommand()=="COMPARE" && gameCount==1)
            {
                cout<<"There is not any other games to compare the current game. Your game will continue from where it was left."<<endl;
                command="Continue";
            }
            else if(getCommand()=="RESET")
            {
                reset();
                setCommand();
            }
            else if(getCommand()=="MENU")
            {
                gameMenu();
            }
	    else if(getCommand()=="LAST")
	    {
		cout<<(lastMove()).getCellColumn()<<(lastMove()).getCellRow()<<"-"<<static_cast<char>((lastMove()).getCellState())<<endl;
	   	setCommand();
	    }
            else if(getCommand()=="CHANGE")
            {
                int newHeight;
                int newWidth;
                string newGameType;

                cout<<"Please enter the height and the width of the new game board you want to create. Board and the width must be the same."<<endl<<
                "Enter height: ";
                cin>>newHeight;
                cin.ignore(1000, '\n');
                if(!cin)
                {
                   cin.clear();
                   cin.ignore(1000, '\n');
                }
                cout<<"Enter width: ";
                cin>>newWidth;
                cin.ignore(1000, '\n');
               if(!cin)
               {
                   cin.clear();
                   cin.ignore(1000, '\n');
               }
               cout<<"Please enter the game type (user vs user  or  user vs computer): "<<endl;
               getline(cin, newGameType);
               cout<<endl;

               for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
                    hexCells[i].~Cell();
                free (hexCells);
                hexCells=nullptr;

               //Initializes the board from the beginning with the changed sizes
               setHexGame(newHeight, newWidth, newGameType);
               setCommand();
            }
            else if(getCommand()!="COMPARE" && getCommand()!="EXIT")
            {
                string numberPart;
                char cInput;
                int nInput;

                cInput=static_cast<char>(command.front());//Gets the first character in the command string as the column input
                if(command.size()==1)//If the command's size is 1, it means it is an invalid one for sure
                    numberPart=-1;
                else
                numberPart=command.substr(1);//If the command's size is not 1, gets the rest of the string as a row input
                for(int i=0; i<numberPart.size(); ++i)
                {
                    if(numberPart[i]>'9' || numberPart[i]<'0')//If the number part of the cell input is not valid, it makes it -1 to be able to change it in validMoveCheck function later on.
                    {
                        numberPart="-1";
                        break;
                    }
                }
                nInput=stoi(numberPart);

                //Sets the given input as the last move, it will be overwritten if it is not a valid move.
                if(whoseTurn=="User1" || whoseTurn=="User")
                    moves[numberOfMoves()].setCell(nInput, cInput, CellState::User1orUser);
                else
                    moves[numberOfMoves()].setCell(nInput, cInput, CellState::User2orComputer);

                controlCell(moves[numberOfMoves()].getCellRow(), moves[numberOfMoves()].getCellColumn());
            }
        }
        catch(CannotOpenFileException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    //Reads the game from the given file
    //Throws CannotOpenFileException if cannot open the given file
    void HexArray1D::readFromFile(string file)
    {
        try
        {
            ifstream readGame(file);

            if(readGame.fail())
                throw CannotOpenFileException();

            //Registers the game related info from the chosen saved game slot

            //Cleans the old board
            for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
                hexCells[i].~Cell();
            free (hexCells);

            readGame>>height>>width;
            readGame.ignore(1000, '\n');
            getline(readGame, gameType);
            readGame>>whoseTurn;
            readGame.ignore(1000, '\n');
            char stateTemp;

            //Allocates memory for the new board
            allocateMemory();

            for(int i=0; i<getBoardHeight(); ++i)
                for(int j=0; j<getBoardWidth(); ++j)
                {
                    readGame>>stateTemp;
                    (hexCells[getBoardWidth()*i+j]).updateCellState(stateTemp);
                }

            readGame>>moveCount;
            for(int i=0; i<numberOfMoves(); ++i)
                readGame>>moves[i];

            readGame.close();
            cout<<endl<<"**********************************"<<endl;
            cout<<endl;
        }
        catch(MemoryAllocationException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    //Writes the game to the given file
    //Throws CannotOpenFileException if cannot open the given file
    void HexArray1D::writeToFile(string file)const
    {
        ofstream writeGame(file);
        if(writeGame.fail())
            throw CannotOpenFileException();

        //Stores the game related info
        writeGame<<getBoardHeight()<<endl<<getBoardWidth()<<endl<<getGameType()<<endl<<getWhoseTurn()<<endl;

        for(int i=0; i<getBoardHeight(); ++i)
            for(int j=0; j<getBoardWidth(); ++j)
                writeGame<<operator()(i,j);

        writeGame<<endl<<numberOfMoves();
        for(int i=0; i<numberOfMoves(); ++i)
            writeGame<<moves[i];

        writeGame.close();

        cout<<"**********************************"<<endl;
        cout<<endl;
    }

    //Checks if the cell input for move is valid
    void HexArray1D::controlCell(int chosenRow, char chosenColumn)
    {
        bool validCheck=false;

        for(int i=0; i<getBoardHeight(); ++i)
            for(int j=0; j<getBoardWidth(); ++j)
                if(i+1==chosenRow && j==(static_cast<int>(chosenColumn-'A')) && hexCells[getBoardWidth()*(chosenRow-1)+(static_cast<int>(chosenColumn-'A'))].getCellState()==CellState::Empty)
                {
                   validCheck=true;
                   moveCount++;//Move count is incremented to save the last move
                }

        if(validCheck==false)
        {
            if(!cin)
            {
                cin.clear();
                cin.ignore(1000,'\n');
            }
            cout<<"Move you wanted to do was not legal, please make another move."<<endl;
            setCommand();
        }
    }

    //Plays one step for the computer
    const Cell& HexArray1D::play()
    {
        int doneMoves=0;
        int j;

        /*The computer makes its move according to the user's previous move. The computer wants to make a move to
        one of the neighbours of the lastChosenCell. Moreover, there is an order of the selection from the neighbours.
        This loop is for making that order*/
        for(int i=1; i<7; ++i)
        {
            switch(i)
            {
            case 1:
                j=2;
                break;
            case 2:
                j=5;
                break;
            case 3:
                j=1;
                break;
            case 4:
                j=4;
                break;
            case 5:
                j=3;
                break;
            case 6:
                j=6;
            }

            try
            {
                //Checks the neighbours one by one according to the selection order. It will stop checking when it finds an empty neighbour and makes a move there.
                if(doneMoves==0 && (lastMove().getNeighbourCell(j)).getNeighbourCellRow()>0 && (lastMove().getNeighbourCell(j)).getNeighbourCellRow()<=height &&
                  (lastMove().getNeighbourCell(j)).getNeighbourCellColumn()>='A' && (lastMove().getNeighbourCell(j)).getNeighbourCellColumn()<=static_cast<char>(width+64) &&
                  hexCells[getBoardWidth()*((lastMove().getNeighbourCell(j)).getNeighbourCellRow()-1)+(static_cast<char>((lastMove().getNeighbourCell(j)).getNeighbourCellColumn())-65)].getCellState()==CellState::Empty)
                {
                    moveCount++;
                    moves[numberOfMoves()-1].setCell((moves[numberOfMoves()-2].getNeighbourCell(j)).getNeighbourCellRow(), (moves[numberOfMoves()-2].getNeighbourCell(j)).getNeighbourCellColumn(), CellState::User2orComputer);//The move is being registered
                    print();
                    hexCells[getBoardWidth()*((lastMove().getCellRow())-1)+(static_cast<char>(lastMove().getCellColumn())-65)].updateCellState('O');//The board is being updated

                    cout<<endl<<"Computer move was "<<lastMove().getCellColumn()<<lastMove().getCellRow()<<endl;

                    doneMoves++;
                    whoseTurn="User";//The turn is being passed

                    return lastMove();
                }
                else if(i==6 && doneMoves==0)//If there is no neighbours to choose from, makes its move to the first empty cell that it finds
                {
                   int j;
                   for(int i=0; i<width; ++i)
                   {
                       for(j=0; j<height; ++j)
                           if(hexCells[getBoardWidth()*j+i].getCellState()==CellState::Empty)
                           {
                               moveCount++;
                               moves[numberOfMoves()-1].setCell(j+1, 'A'+i, CellState::User2orComputer);//The move is being registered
                               print();
                               hexCells[getBoardWidth()*j+i].updateCellState('O');//The board is being updated

                               cout<<"Computer move was "<<static_cast<char>(i+65)<<j+1<<endl;
                               whoseTurn="User";//The turn is being passed

                               return lastMove();
                           }
                   }
                }
            }
            catch(NoMoveException& exc)
            {
                cerr<<exc.what()<<endl;
                exit(-1);
            }
        }
    }

    //Plays one step for the user
    const Cell& HexArray1D::play(Cell cMove)
    {
        try{
            if(whoseTurn=="User1" || whoseTurn=="User")
            {
                (hexCells[getBoardWidth()*(cMove.getCellRow()-1)+((static_cast<int>(cMove.getCellColumn()))-65)]).updateCellState('X');//The board is being updated
                cout<<"The move was "<<lastMove().getCellColumn()<<lastMove().getCellRow()<<endl;

                whoseTurn="User2";//The turn is being passed

                return lastMove();
            }
            else if(whoseTurn=="User2")
            {
                (hexCells[getBoardWidth()*(cMove.getCellRow()-1)+((static_cast<int>(cMove.getCellColumn()))-65)]).updateCellState('O');//The board is being updated
                cout<<"The move was "<<lastMove().getCellColumn()<<lastMove().getCellRow()<<endl;

                whoseTurn="User1";//The turn is being passed

                return lastMove();
            }
        }
        catch(NoMoveException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    //Starts playing the game
    void HexArray1D::playGame()
    {
        if(whoseTurn=="Start")
        {
            int newHeight;
            int newWidth;
            string newGameType;

            if(hexCells!=nullptr)
            {
                for(int i=0; i<getBoardHeight()*getBoardWidth(); ++i)
                    hexCells[i].~Cell();

                free (hexCells);
                hexCells=nullptr;
            }

            cout<<"Please enter the height and the width of the game board you want to create. Board and the width must be the same."<<endl<<
            "Enter height: ";
            cin>>newHeight;
            cin.ignore(1000, '\n');
            if(!cin)
            {
               cin.clear();
               cin.ignore(1000, '\n');
            }
            cout<<"Enter width: ";
            cin>>newWidth;
            cin.ignore(1000, '\n');
           if(!cin)
           {
               cin.clear();
               cin.ignore(1000, '\n');
           }
           cout<<"Please enter the game type (user vs user  or  user vs computer): "<<endl;
           getline(cin, newGameType);

           setHexGame(newHeight, newWidth, newGameType);
           setBoard();
        }

        if(getWhoseTurn()=="Start" && getGameType()=="user vs user")
            whoseTurn="User1";
        else if(getWhoseTurn()=="Start" && getGameType()=="user vs computer")
            whoseTurn="User";

        Cell chosenMove;

        setCommand();

        try
        {
            if(getGameType()=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE" && getCommand()!="EXIT")
            {
                chosenMove=lastMove();
                play(chosenMove);//Plays for one move if there is nothing that breaks the game's flow

                if(isEnd()==false)
                {
                    setCommand();
                    chosenMove=lastMove();
                    if(gameType=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE" && getCommand()!="EXIT")
                        play(chosenMove);//The other user also plays for one move if there is nothing that breaks the game's flow
                }
            }
            if(getGameType()=="user vs computer" && getCommand()!="MENU" && getCommand()!="COMPARE" && getCommand()!="EXIT")
            {
                chosenMove=lastMove();
                play(chosenMove);

                if(isEnd()==false)
                    play();
            }

            //Ends the game if there is a win condition
            if((isEnd() && getGameType()=="user vs user") || (getGameType()=="user vs computer" && getWhoseTurn()!="User"))
            {
                print();
                cout<<endl<<"You have won."<<endl;
            }
            else if(isEnd() && getGameType()=="user vs computer" && getWhoseTurn()=="User")
            {
                print();
                cout<<endl<<"Computer has won."<<endl;
            }
        }
        catch(NoMoveException& exc)
        {
            cerr<<exc.what()<<endl;
            exit(-1);
        }
    }

    //Checks if there is a win condition
    bool HexArray1D::isEnd()const
    {
        int pastCount=0;
        vector <Cell> pastCells;
        //Past cells is to store the cells the function has already checked before. The function should not visit them again
        pastCells.resize(width*height);

        for(int i=0; i<height; ++i)//Searchs for a cell to start checking from on the very left side
            if(hexCells[getBoardWidth()*i].getCellState()==CellState::User1orUser)
            {
                pastCells[0].setCell(i+1, 'A');
                pastCount++;
                if(neighbourCheck(i+1, 'A', CellState::User1orUser, pastCount, pastCells))//Checks for a path of connected X that leads to a win condition
                    return true;
                pastCount=0;
            }

        for(int j=0; j<width; ++j)//Searchs for a cell to start checking from at the top
            if(hexCells[getBoardWidth()*0+j].getCellState()==CellState::User2orComputer)
            {
                pastCells[0].setCell(1, 'A'+j);
                pastCount++;
                if(neighbourCheck(1, 'A'+j, CellState::User2orComputer, pastCount, pastCells))//Checks for a path of connected O that leads to a win condition
                    return true;
                pastCount=0;
            }

        return false;
    }

    bool HexArray1D::neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, vector<Cell> &pastCells)const
    {
        Cell startCell;
        startCell.setCell(startRow, startColumn, stateInfo);

        /*Checks the startCell's neighbours, if one of the neighbour's state is the same as the given stateInfo, it registers that neighbour as the new startCell
        also registers as a past cell not to visit that cell again.
        If it fails to find a path, returns back to the closest cell with other neighbours that has the same state.
        Keeps checking till there is no more neighbours to register as the startCell*/
        for(int j=1; j<7; ++j)
        {
           if((startCell.getNeighbourCell(j)).getNeighbourCellRow()>0 && (startCell.getNeighbourCell(j)).getNeighbourCellRow()<=height &&
           (startCell.getNeighbourCell(j)).getNeighbourCellColumn()>='A' && (startCell.getNeighbourCell(j)).getNeighbourCellColumn()<=static_cast<char>(width+64) &&
           hexCells[getBoardWidth()*((startCell.getNeighbourCell(j)).getNeighbourCellRow()-1)+(static_cast<char>((startCell.getNeighbourCell(j)).getNeighbourCellColumn())-65)].getCellState()==stateInfo)
           {
              for(int i=0; i<pastCount; ++i)
              {
                 if((pastCells[i].getCellRow())==(startCell.getNeighbourCell(j)).getNeighbourCellRow() && (pastCells[i].getCellColumn())==(startCell.getNeighbourCell(j)).getNeighbourCellColumn())
                    break;//Does not registers the cell if the cell was found as a past cell
                 else if(i==pastCount-1)
                 {
                    pastCells[pastCount].setCell((startCell.getNeighbourCell(j)).getNeighbourCellRow(), (startCell.getNeighbourCell(j)).getNeighbourCellColumn());
                    pastCount++;
                    if(neighbourCheck((startCell.getNeighbourCell(j)).getNeighbourCellRow(), (startCell.getNeighbourCell(j)).getNeighbourCellColumn(), stateInfo, pastCount, pastCells))
                        return true;
                 }
               }
            }
        }
        if(startCell.getCellState()==CellState::User1orUser && startCell.getCellColumn()==static_cast<char>(width+64))
            return true;//If the path was reached to the very right side, it means there is a win condition
        else if(startCell.getCellState()==CellState::User2orComputer && startCell.getCellRow()==height)
            return true;//If the path was reached to the bottom, it means there is a win condition

        return false;
    }
}
