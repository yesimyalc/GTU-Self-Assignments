#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include "Hex.h"

using namespace std;

Hex::Cell::NeighbourCell::NeighbourCell()  : nRow(1), nColumn('A')
{/*Intenionally left empty*/}

void Hex::Cell::NeighbourCell::setNeighbourCell(int nRowNumber, char nColumnPosition)
{
    nRow=nRowNumber;
    nColumn=nColumnPosition;
}

Hex::Cell::Cell()
{
    setCell(0, 'A', CellState::Empty);
}

//Big Three
Hex::Cell::Cell(const Cell& other)
{
    setCell(other.getCellRow(), other.getCellColumn(), other.getCellState());
}

Hex::Cell::~Cell()
{
    delete[] neighbourCells;
}

const Hex::Cell& Hex::Cell::operator=(const Cell& other)
{
    if(this==&other)
        return *this;
    else
    {
        setCell(other.getCellRow(), other.getCellColumn(), other.getCellState());
        return *this;
    }
}

void Hex::Cell::setCell(int rowNumber, char columnPosition, CellState stateInfo)
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
void Hex::Cell::setCell(int rowNumber, char columnPosition)
{
    row=rowNumber;
    column=columnPosition;
}

//Sets the cell's 6 neighbours. There might be neighbours outside of the game's board too but they will be ignored later on
void Hex::Cell::setNeighbourCells()
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
void Hex::Cell::updateCellState(char newState)
{
    if(newState=='X')
        state=CellState::User1orUser;
    else if(newState=='O')
        state=CellState::User2orComputer;
    else
        state=CellState::Empty;
}

//Only prints out the cell's position and the state because other informations about the cells can stay hidden, they are not useful to print out
ostream& operator<<(ostream& outputStream, const Hex::Cell& writtenCell)
{
    outputStream<<endl<<writtenCell.getCellColumn()<<writtenCell.getCellRow()<<"\t"<<static_cast<char>(writtenCell.getCellState());
    return outputStream;
}

istream& operator>>(istream& inputStream, Hex::Cell& readCell)
{
    char temp;
    inputStream>>readCell.column>>readCell.row>>temp;
    readCell.updateCellState(temp);
    readCell.setNeighbourCells();

    return inputStream;
}

Hex::Hex(int heightValue, int widthValue, string gameTypeInfo)//Initializes as the given values
                    : height(heightValue), width(widthValue), gameType(gameTypeInfo), whoseTurn("Start")
{
    setHexGame();
}

Hex::Hex(string gameTypeInfo)//initializes as the given game type and min board sizes
                    : height(6), width(6), gameType(gameTypeInfo), whoseTurn("Start")
{
    setHexGame();
}

Hex::Hex() :height(6), width(6), gameType("user vs computer"), whoseTurn("Start")//Initializes as min board sizes and computer vs player
{
    setHexGame();
}

//Big Three
Hex::Hex(const Hex& other)
{
    *this=other;
}

Hex::~Hex()
{
    for(int i=0; i<getBoardHeight(); ++i)
        delete[] hexCells[i];
    delete[] hexCells;

    delete[] moves;
}

const Hex& Hex::operator =(const Hex& other)
{
    if(this==&other)
        return *this;
    else
    {
        gameType=other.getGameType();
        whoseTurn=other.getWhoseTurn();

        if(hexCells!=nullptr && (getBoardHeight()!=other.getBoardHeight() || getBoardWidth()!=other.getBoardWidth()))
        {
            //Deletes the old hex board if necessary
            for(int i=0; i<getBoardHeight(); ++i)
                delete[] hexCells[i];
            delete[] hexCells;
            hexCells=nullptr;

            if(getBoardHeight()*getBoardWidth()!=other.getBoardHeight()*other.getBoardWidth())
            {
                //Deletes the old moves if necessary
                delete[] moves;
                moves=nullptr;
            }
        }
        if(hexCells==nullptr)
        {
            hexCells=new Cell*[other.getBoardHeight()];
            if(hexCells==nullptr)
            {
                cerr<<"Cannot allocate memory."<<endl;
                exit(-1);
            }

            for(int i=0; i<other.getBoardHeight(); ++i)
            {
                hexCells[i]=new Cell[other.getBoardWidth()];
                if(hexCells[i]==nullptr)
                {
                    cerr<<"Cannot allocate memory."<<endl;
                    exit(-1);
                }
            }
        }

        height=other.height;
        width=other.width;

        //Sets the new board according to the new board values
        for(int i=0; i<getBoardHeight(); ++i)
            for(int j=0; j<getBoardWidth(); ++j)
                (hexCells[i][j]).updateCellState(static_cast<char>((other.hexCells[i][j]).getCellState()));

        xCount=other.xCount;
        oCount=other.oCount;
        chosenCell.setCell(other.chosenCell.getCellRow(), other.chosenCell.getCellColumn());
        moveCount=other.getMoveCount();
        xScore=other.getXScore();
        oScore=other.getOScore();

        //If there were no moves done from before, allocates space for the new moves
        if(moves==nullptr)
            moves=new Cell[other.getBoardHeight()*getBoardWidth()];
            if(moves==nullptr)
            {
                cout<<"Cannot allocate memory."<<endl;
                exit(-1);
            }
        for(int i=0; i<getMoveCount(); ++i)
            moves[i]=other.moves[i];

        return *this;
    }
}

//Prints out all the information about the hex class, can be used for files and screen prints
ostream& operator<<(ostream& outputStream, const Hex& writtenHex)
{
    outputStream<<writtenHex.getBoardHeight()<<endl<<writtenHex.getBoardWidth()<<endl<<writtenHex.getGameType()<<endl<<writtenHex.getWhoseTurn()<<endl<<
    writtenHex.getAllMarkedCells()<<endl<<writtenHex.getXCount()<<endl<<writtenHex.getOCount()<<endl;

    for(int i=0; i<writtenHex.getBoardHeight(); ++i)
        for(int j=0; j<writtenHex.getBoardWidth(); ++j)
            outputStream<< static_cast<char> ((writtenHex.hexCells[i][j]).getCellState());

    outputStream<<endl<<writtenHex.getMoveCount();
    for(int i=0; i<writtenHex.getMoveCount(); ++i)
        outputStream<<writtenHex.moves[i];

    outputStream<<endl<<writtenHex.getXScore()<<endl<<writtenHex.getOScore();

    return outputStream;
}

//To read from files only 
ifstream& operator>>(ifstream& inputStream, Hex& readGame)
{
    int markedCellTemp;

    //Resetting the old board
    for(int i=0; i<readGame.getBoardHeight(); ++i)
        delete[] readGame.hexCells[i];
    delete[] readGame.hexCells;

    Hex::allMarkedCells-=(readGame.getXCount()+readGame.getOCount());
    inputStream>>readGame.height>>readGame.width;
    inputStream.ignore(1000, '\n');
    getline(inputStream, readGame.gameType);
    inputStream>>readGame.whoseTurn>>markedCellTemp>>readGame.xCount>>readGame.oCount;
    inputStream.ignore(1000, '\n');
    Hex::allMarkedCells+=markedCellTemp;

    char stateTemp;

    //Resizes the board according to the new height and width
    readGame.hexCells=new Hex::Cell*[readGame.getBoardHeight()];
    if(readGame.hexCells==nullptr)
    {
        cout<<"Cannot allocate memory."<<endl;
        exit(-1);
    }
    for(int i=0; i<readGame.getBoardHeight(); ++i)
    {
        readGame.hexCells[i]=new Hex::Cell[readGame.getBoardWidth()];
        if(readGame.hexCells[i]==nullptr)
        {
            cout<<"Cannot allocate memory."<<endl;
            exit(-1);
        }
    }

    //Updates the board
    for(int i=0; i<readGame.getBoardHeight(); ++i)
        for(int j=0; j<readGame.getBoardWidth(); ++j)
        {
            inputStream>>stateTemp;
            readGame.hexCells[i][j].updateCellState(stateTemp);
        }

    //Deletes old moves and registers the new moves
    delete[] readGame.moves;
    readGame.moves=new Hex::Cell[readGame.getBoardHeight()*readGame.getBoardWidth()];
    if(readGame.moves==nullptr)
    {
        cout<<"Cannot allocate memory."<<endl;
        exit(-1);
    }
    inputStream>>readGame.moveCount;
    for(int i=0; i<readGame.getMoveCount(); ++i)
        inputStream>>readGame.moves[i];

    inputStream>>readGame.xScore>>readGame.oScore;

    return inputStream;
}

//Pre decrement
Hex Hex::operator--()
{
    /*If the gameType is user vs user, deletes the latest move only and returns the turn to whoever's move was deleted.
    If the gameType is user vs computer, deletes the two latest moves. 1 from the computer and 1 from the user. Because
    if only the computer's move is deleted, the computer will make the same move again and again.*/
    if(moveCount==0)
    {
        cerr<<"There are no moves that can be deleted."<<endl;
        exit(-1);
    }
    if(getGameType()=="user vs user" && getWhoseTurn()=="User1")
    {
        hexCells[((moves[getMoveCount()-1]).getCellRow())-1][static_cast<int>(((moves[getMoveCount()-1]).getCellColumn())-'A')].updateCellState('.');
        moveCount--;
	oCount--;
	allMarkedCells--;
   	//Score will be calculated from the beginning
        oScore=0;
        winCheck();
        whoseTurn="User2";
    }
    else if(getGameType()=="user vs user" && getWhoseTurn()=="User2")
    {
        hexCells[((moves[getMoveCount()-1]).getCellRow())-1][static_cast<int>(((moves[getMoveCount()-1]).getCellColumn())-'A')].updateCellState('.');
        moveCount--;
	xCount--;
	allMarkedCells--;
	//Score will be calculated from the beginning
	xScore=0;
        winCheck();
        whoseTurn="User1";
    }
    else
    {
        hexCells[((moves[getMoveCount()-1]).getCellRow())-1][static_cast<int>(((moves[getMoveCount()-1]).getCellColumn())-'A')].updateCellState('.');
        hexCells[((moves[getMoveCount()-2]).getCellRow())-1][static_cast<int>(((moves[getMoveCount()-2]).getCellColumn())-'A')].updateCellState('.');
        oCount--;
        xCount--;
        allMarkedCells-=2;
        moveCount-=2;
	//Scores will be calculated from the beginning
	xScore=0;
        oScore=0;
        winCheck();
    }

    Hex temp=*this;

    return temp;

}

//Post decrement
Hex Hex::operator--(int ignore)
{
    //Does the same thing with --operator, however returns the old hex game.
    Hex temp=*this;

    --*this;

    return temp;
}

/*Compares two existing games. If the current game has more occupied cells for whoever has the turn (user1 or user2), the first game wins over the other game, returns true.
If one of the game's game type is user vs computer, for that game the user's occupied cells will be counted*/
bool Hex::operator==(const Hex& otherGame)const
{
    if((getWhoseTurn()=="User1" || getWhoseTurn()=="User") && getXCount()>otherGame.getXCount())
        return true;
    else if(getWhoseTurn()=="User2" && otherGame.getGameType()=="user vs user" && getOCount()>otherGame.getOCount())
        return true;
    else if(getWhoseTurn()=="User2" && otherGame.getGameType()=="user vs computer" && getOCount()>otherGame.getXCount())
        return true;

    return false;
}

/*Checks if the height & width & game type values are valid.
Also does some initializations*/
void Hex::setHexGame()
{
    setHexGameBoard();
    while(gameType!="user vs user" && gameType!="user vs computer")
    {
        cout<<"You have entered invalid game type, please enter again."<<endl;
        getline(cin, gameType);
    }

    chosenCell.setCell(1, 'A', CellState::Empty);

    moves=new Cell[height*width];
    if(moves==nullptr)
    {
        cout<<"Cannot allocate memory."<<endl;
        exit(-1);
    }

    moveCount=0;
    xCount=0;
    oCount=0;
    xScore=0;
    oScore=0;
}

//Sets the game's board according to the height and width values, also checks the values
void Hex::setHexGameBoard()
{
    while(height<6 || width<6 || height!=width)
    {
        cout<<"You have entered invalid height and width values, please enter again."<<endl<<
        "Enter height: ";
        cin>>height;
        cin.ignore(1000, '\n');
        if(!cin)
        {
           cin.clear();
           cin.ignore(1000, '\n');
        }
        cout<<"Enter width: ";
        cin>>width;
        cin.ignore(1000, '\n');
        if(!cin)
        {
           cin.clear();
           cin.ignore(1000, '\n');
        }
    }

    //Resizes the board according to the height and width values
    hexCells=new Cell*[getBoardHeight()];
    if(hexCells==nullptr)
    {
        cerr<<"Cannot allocate memory."<<endl;
        exit(-1);
    }

    for(int i=0; i<getBoardHeight(); ++i)
    {
        hexCells[i]=new Cell[getBoardWidth()];
        if(hexCells[i]==nullptr)
        {
            cerr<<"Cannot allocate memory."<<endl;
            exit(-1);
        }
    }

    for(int i=0; i<getBoardHeight(); ++i)//Registers all the cells on the board as empty
        for(int j=0; j<getBoardWidth(); ++j)
            hexCells[i][j].setCell(i+1, 'A'+j, CellState::Empty);
}

//This is used when the game information like height and width is changed outside of the constructors
void Hex::setHexGame(int newHeightValue, int newWidthValue)
{
    for(int i=0; i<getBoardHeight(); ++i)
        delete[] hexCells[i];
    delete[] hexCells;

    delete[] moves;

    height=newHeightValue;
    width=newWidthValue;
    setHexGameBoard();

    moves=new Cell[height*width];
    if(moves==nullptr)
    {
        cout<<"Cannot allocate memory."<<endl;
        exit(-1);
    }
}

//Displays the board
void Hex::displayBoard()const
{
    int l;

    for(int i=0; i<getBoardHeight(); ++i)
      for(int j=0; j<getBoardWidth(); ++j)
      {
        if(i==0 && j==0)
        {
            cout<<"  ";
            for(l=0; l<getBoardWidth(); l++)
              cout<<" "<<static_cast<char>('A'+l);
        }

        if(j==0)
        {
            cout<<endl<<i+1;
              for(l=0; l<i+1; ++l)
                cout<<" ";
        }
        cout<<" "<<static_cast<char>(hexCells[i][j].getCellState());
       }
       cout<<endl;
}

//Saves the game to a chosen saved game slot
void Hex::saveGame()
{
    cout<<endl<<"**********Save Game Menu**********"<<endl;

    string fileName;

    cout<<"You can type the saved game file name you want and and save a game onto it."<<endl<<
    "If you write an already existed saved game file name, the game will be overwritten."<<endl<<
    "The saved game file name format should be like this: name.txt or name.TXT, name can be anything you want."<<endl;

    getline(cin, fileName);

    string temp;
    temp=fileName.substr((fileName.size()-4), 4);

    while(temp!=".txt" && temp!=".TXT")
    {
        cout<<"You have entered an invalid file name type. Please enter again."<<endl;
        getline(cin, fileName);

        temp=fileName.substr((fileName.size()-4), 4);
    }

    ofstream writeGame(fileName);
    if(writeGame.fail())
    {
        cerr<<"File cannot be opened."<<endl;
        exit(-1);
    }

    //Stores the game related info
    writeGame<<*this;

    writeGame.close();

    cout<<"**********************************"<<endl;
    cout<<endl;
}

//Loads the game from a chosen saved game slot
void Hex::loadGame()
{
    cout<<endl<<"**********Load Game Menu**********"<<endl;

    string fileName;

    cout<<"Enter the saved game file you want to open. Do not forget to include .txt or .TXT in the file name."<<endl;

    cin>>fileName;
    cin.ignore(1000, '\n');

    ifstream readGame(fileName);
    if(readGame.fail())
    {
        cerr<<endl<<"You have tried to load from a non existing saved game file."<<endl;
        exit(-1);
    }

    //Registers the game related info from the chosen saved game slot
    readGame>>*this;

    readGame.close();
    cout<<endl<<"**********************************"<<endl;
    cout<<endl;
}

//Plays the game for a one move and passes the turn to the opponent
Hex::Cell& Hex::play(Cell cMove)
{
    if(whoseTurn=="User1" || whoseTurn=="User")
    {
        (hexCells[cMove.getCellRow()-1][(static_cast<int>(cMove.getCellColumn()))-65]).updateCellState('X');//The board is being updated
        moveCount++;
        moves[getMoveCount()-1].setCell(cMove.getCellRow(), cMove.getCellColumn(), CellState::User1orUser);//The move is being registered

        cout<<"The move was "<<moves[getMoveCount()-1].getCellColumn()<<moves[getMoveCount()-1].getCellRow()<<endl;

        whoseTurn="User2";//The turn is being passed
        xCount++;
        allMarkedCells++;

        return moves[getMoveCount()-1];
    }
    else if(whoseTurn=="User2")
    {
        (hexCells[cMove.getCellRow()-1][(static_cast<int>(cMove.getCellColumn()))-65]).updateCellState('O');//The board is being updated
        moveCount++;
        moves[getMoveCount()-1].setCell(cMove.getCellRow(), cMove.getCellColumn(), CellState::User2orComputer);//The move is being registered

        cout<<"The move was "<<moves[getMoveCount()-1].getCellColumn()<<moves[getMoveCount()-1].getCellRow()<<endl;

        whoseTurn="User1";//The turn is being passed
        oCount++;
        allMarkedCells++;

        return moves[getMoveCount()-1];
    }
}

//Plays the game for one move for computer and passes the turn to User
Hex::Cell& Hex::play()
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

        //Checks the neighbours one by one according to the selection order. It will stop checking when it finds an empty neighbour and makes a move there.
        if(doneMoves==0 && (moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellRow()>0 && (moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellRow()<=height &&
          (moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellColumn()>='A' && (moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellColumn()<=static_cast<char>(width+64) &&
          hexCells[(moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellRow()-1][static_cast<char>((moves[getMoveCount()-1].getNeighbourCell(j)).getNeighbourCellColumn())-65].getCellState()==CellState::Empty)
        {
            moveCount++;
            moves[getMoveCount()-1].setCell((moves[getMoveCount()-2].getNeighbourCell(j)).getNeighbourCellRow(), (moves[getMoveCount()-2].getNeighbourCell(j)).getNeighbourCellColumn(), CellState::User2orComputer);//The move is being registered
            displayBoard();
            hexCells[(moves[getMoveCount()-1].getCellRow())-1][static_cast<char>(moves[getMoveCount()-1].getCellColumn())-65].updateCellState('O');//The board is being updated

            cout<<endl<<"Computer move was "<<moves[getMoveCount()-1].getCellColumn()<<moves[getMoveCount()-1].getCellRow()<<endl;

            doneMoves++;
            oCount++;
            allMarkedCells++;
            whoseTurn="User";//The turn is being passed

            return moves[getMoveCount()-1];
        }
        else if(i==6 && doneMoves==0)//If there is no neighbours to choose from, makes its move to the first empty cell that it finds
        {
           int j;
           for(int i=0; i<width; ++i)
           {
               for(j=0; j<height; ++j)
                   if(hexCells[j][i].getCellState()==CellState::Empty)
                   {
                       moveCount++;
		       oCount++;
                       moves[getMoveCount()-1].setCell(j+1, 'A'+i, CellState::User2orComputer);//The move is being registered
                       displayBoard();
                       hexCells[j][i].updateCellState('O');//The board is being updated

                       cout<<"Computer move was "<<static_cast<char>(i+65)<<j+1<<endl;
                       whoseTurn="User";//The turn is being passed

                       return moves[getMoveCount()-1];
                   }
           }
        }
    }
}

//Checks the validity of the move input from the user
void Hex::controlCell(int& chosenRow, char& chosenColumn)
{
    //A move is valid if the chosen cell is empty and inside the board
    bool validCheck=false;

    for(int i=0; i<getBoardHeight(); ++i)
        for(int j=0; j<getBoardWidth(); ++j)
            if(i+1==chosenRow && j==(static_cast<int>(chosenColumn-'A')) && hexCells[chosenRow-1][(static_cast<int>(chosenColumn-'A'))].getCellState()==CellState::Empty)
               validCheck=true;

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

//Gets the move input or other commands from the user
void Hex::setCommand()
{
    string numberPart;
    char cInput;
    int nInput;

    displayBoard();
    cout<<endl<<getWhoseTurn()<<"'s Turn"<<endl;
    cout<<"Make your move: "<<endl;
    getline(cin, command);

    //If input command is not one of these, it means it is either an invalid one or a cell input. Converts the command to a proper cell input.
    if(command=="SCORE")
    {
        cout<<"Your score is "<<getUserScore()<<"."<<endl;
        setCommand();
    }
    else if(command=="SAVE FILE.TXT")
    {
        saveGame();
        setCommand();
    }
    else if(command=="LOAD FILE.TXT")
    {
        loadGame();
        setCommand();
    }
    else if(command=="MENU")
        gameMenu();
    else if(command=="COMPARE" && gameCount==1)
    {
        cout<<"There is not any other games to compare the current game. Your game will continue from where it was left."<<endl;
        command="Continue";
    }
    else if(command=="UNDO--")//Does undo with post decrement operator
    {
        (*this)--;
        setCommand();
    }
    else if(command=="--UNDO")//Does undo with pre decrement operator
    {
        --(*this);
        setCommand();
    }
    else if(command!="COMPARE")
    {
        //If input command is not one of these, it means it is either an invalid one or a cell input. Converts the command to a proper cell input.
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
        chosenCell.setCell(nInput, cInput);//Sets the chosenCell as the move that is going to be done
        controlCell(nInput, cInput);//However it needs to be checked for validation before, the chosenCell might be registered again
    }
}

//Game menu is for switching between games
void Hex::gameMenu()
{
    cout<<endl<<"**********Game Menu**********"<<endl;
    int temp;
    cout<<endl<<"There are "<<gameCount<<" games opened. Which game do you want to switch to?"<<endl<<
    currentGameNo<<"-) Current Game"<<endl;
        for(int i=0; i<gameCount; ++i)
            if(i+1!=currentGameNo)
               cout<<i+1<<"-) Game "<<i+1<<endl;
    temp=currentGameNo;
    cin>>currentGameNo;
    cin.ignore(1000,'\n');
    while(currentGameNo<1 || currentGameNo>gameCount)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid number. Please enter again."<<endl;
        cin>>currentGameNo;
        cin.ignore(1000,'\n');
    }

    if(currentGameNo==temp)
    {
        cout<<"You have entered you old game number. The game will continue from where it was left."<<endl;
        setCommand();
    }
    cout<<endl<<"*****************************"<<endl;
}

/*This function firstly sets the game's width and height if it was called for the first time.
After that plays the game for User1->User2 or User->Computer for once*/
void Hex::playGame()
{
    if(whoseTurn=="Start")
    {
        int newHeight;
        int newWidth;
        cout<<endl<<"Please enter the height and the width of the game board you want to create. Board and the width must be the same."<<endl<<
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
       setHexGame(newHeight, newWidth);
    }

    if(getWhoseTurn()=="Start" && getGameType()=="user vs user")
        whoseTurn="User1";
    else if(getWhoseTurn()=="Start" && getGameType()=="user vs computer")
        whoseTurn="User";

    Cell chosenMove;
    setCommand();
    /*It needs to exit from getting commands from the users if any of the users enters MENU or COMPARE commands because those commands
    might change the game's flow*/
    if(getGameType()=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE")
    {
        chosenMove=chosenCell;
        play(chosenMove);//Plays for one move if there is nothing that breaks the game's flow
        if(winCheck()==false)//It also checks if there is a win condition after each move
        {
           setCommand();
           chosenMove=chosenCell;
           if(gameType=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE")
              play(chosenMove);//The other user also plays for one move if there is nothing that breaks the game's flow
        }
    }
    if(getGameType()=="user vs computer" && getCommand()!="MENU" && getCommand()!="COMPARE")
    {
       chosenMove=chosenCell;
       play(chosenMove);
       if(winCheck()==false)
        play();
    }

    //Ends the game if there is a win condition
    if(winCheck() && getGameType()=="user vs user" || (getGameType()=="user vs computer" && getWhoseTurn()!="User"))
    {
        displayBoard();
        cout<<endl<<"You have won."<<endl;
	xScore+=100;//Extra 100 points if wins
    }
    else if(winCheck() && getGameType()=="user vs computer" && getWhoseTurn()=="User")
    {
        displayBoard();
        cout<<endl<<"Computer has won."<<endl;
	oScore+=100;//Extra 100 points if wins
    }
}

//Checks if there is a win condition for any of the users or computer. Returns true if there is
bool Hex::winCheck()
{
    int pastCount=0;
    int scoreTemp;
    Cell* pastCells;
    pastCells=new Cell[width*height];
    if(pastCells==nullptr)
    {
        cout<<"Cannot allocate memory."<<endl;
        exit(-1);
    }
    //Past cells is to store the cells the function has already checked before. The function should not visit them again

    for(int i=0; i<height; ++i)//Searchs for a cell to start checking from on the very left side
        if(hexCells[i][0].getCellState()==CellState::User1orUser)
        {
            pastCells[0].setCell(i+1, 'A');
            pastCount++;
            scoreTemp=xScore;
            xScore=1;
            /*Players gain 1 score if they have a starting cell, because it means there is a start for a path that leads to win condition.
            If there is no start, their score will stay as 0 even though they have many cells occupied. Because even if they have many cells occupied,
            it does not mean much. If they do not have any cells they have as a path start point, their path can be blocked easily.*/
            if(neighbourCheck(i+1, 'A', CellState::User1orUser, pastCount, pastCells))//Checks for a path of connected X that leads to a win condition
                return true;
            pastCount=0;
            if(scoreTemp>xScore)//Will keep the maximum score
                xScore=scoreTemp;
        }

    for(int j=0; j<width; ++j)//Searchs for a cell to start checking from at the top
        if(hexCells[0][j].getCellState()==CellState::User2orComputer)
        {
            pastCells[0].setCell(1, 'A'+j);
            pastCount++;
            scoreTemp=oScore;
            oScore=1;
            if(neighbourCheck(1, 'A'+j, CellState::User2orComputer, pastCount, pastCells))//Checks for a path of connected O that leads to a win condition
                return true;
            pastCount=0;
            if(scoreTemp>oScore)//Will keep the maximum score
                oScore=scoreTemp;
        }
    delete[] pastCells;

    return false;
}

//Used in winCheck. Checks if there is a path that leads to a win condition. Returns true if there is
bool Hex::neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, Cell* pastCells)
{
    Cell startCell;
    startCell.setCell(startRow, startColumn, stateInfo);

    /*Checks the startCell's neighbours, if one of the neighbour's state is the same as the given stateInfo, it registers that neighbour as the new startCell
    also registers as a past cell not to visit that cell again.
    If it fails to find a path, returns back to the closest cell with other neighbours that has the same state.
    Keeps checking till there is no more neighbours to register as the startCell

    Also keeps a record for the scores of each player while visiting neighbour cells of the starting cell. Each neighbour that can be visited will make the player gain +1 score.
    It means players get more scores for longer connected paths starting from top for user2 and computer, starting from left side for user1 and user.*/
    for(int j=1; j<7; ++j)
    {
       if((startCell.getNeighbourCell(j)).getNeighbourCellRow()>0 && (startCell.getNeighbourCell(j)).getNeighbourCellRow()<=height &&
       (startCell.getNeighbourCell(j)).getNeighbourCellColumn()>='A' && (startCell.getNeighbourCell(j)).getNeighbourCellColumn()<=static_cast<char>(width+64) &&
       hexCells[(startCell.getNeighbourCell(j)).getNeighbourCellRow()-1][static_cast<char>((startCell.getNeighbourCell(j)).getNeighbourCellColumn())-65].getCellState()==stateInfo)
       {
          for(int i=0; i<pastCount; ++i)
          {
             if((pastCells[i].getCellRow())==(startCell.getNeighbourCell(j)).getNeighbourCellRow() && (pastCells[i].getCellColumn())==(startCell.getNeighbourCell(j)).getNeighbourCellColumn())
                break;//Does not registers the cell if the cell was found as a past cell
             else if(i==pastCount-1)
             {
                 //Registers the cell if it was not a past cell before and continues
                 pastCells[pastCount].setCell((startCell.getNeighbourCell(j)).getNeighbourCellRow(), (startCell.getNeighbourCell(j)).getNeighbourCellColumn());
                 pastCount++;

                 if(stateInfo==CellState::User1orUser)
                    xScore++;
                 else
                    oScore++;

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

//Returns the current player's score
int Hex::getUserScore()const
{
    if(getWhoseTurn()=="User1" || getWhoseTurn()=="User")
        return getXScore();
    else
        return getOScore();
}
