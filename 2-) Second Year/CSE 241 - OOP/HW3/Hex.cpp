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

Hex::Cell::Cell()  : row(0), column('A'), state(CellState::Empty)
{
    neighbourCells.resize(6);
}

void Hex::Cell::setCell(int rowNumber, char columnPosition, CellState stateInfo)
{
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

//Updates the cell's state when that cell was chosen as a move or in a load game condition
void Hex::Cell::updateCellState(char newState)
{
    if(newState=='X')
        state=CellState::User1orUser;
    else if(newState=='O')
        state=CellState::User2orComputer;
    else
        state=CellState::Empty;
}

Hex::Hex(int heightValue, int widthValue, string gameTypeInfo)
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

/*Checks if the height & width & game type values are valid.
Also resizes, fills vectors etc*/
void Hex::setHexGame()
{
    setHexGameBoard();
    while(gameType!="user vs user" && gameType!="user vs computer")
    {
        cout<<"You have entered invalid game type, please enter again."<<endl;
        getline(cin, gameType);
    }

    string fileName;
    fullSavedGameSlots.resize(20);

    lastChosenCell.setCell(1, 'A', CellState::Empty);
    chosenCell.setCell(1, 'A', CellState::Empty);

    xCount=0;
    oCount=0;
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

    hexCells.resize(height);//Resizes the board according to the height and width values
    for(int i=0; i<height; ++i)
        hexCells[i].resize(width);

    for(int i=0; i<height; ++i)//Registers all the cells on the board as empty
        for(int j=0; j<width; ++j)
            hexCells[i][j].setCell(i+1, 'A'+j, CellState::Empty);
}

/*This is used when the game information like height and width is changed outside of the constructors
Only changes the board size and adds, removes cells*/
void Hex::setHexGame(int newHeightValue, int newWidthValue)
{
    height=newHeightValue;
    width=newWidthValue;
    setHexGameBoard();
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

//Updates the saved game slot numbers in a vector to see while saving or loading a game. Also prints them on the screen.
void Hex::getSavedGames()
{
    string fileName;
    cout<<endl;
    for(int i=0; i<20; ++i)
    {
        fileName="SavedGame"+to_string(i+1)+".txt";
        ifstream file(fileName);

        if(file)
        {
            fullSavedGameSlots[i]=i+1;
            cout<<i+1<<"-)"<<fileName<<endl;
            file.close();
        }
        else
            fullSavedGameSlots[i]=-1;
    }
    cout<<endl;
}

//Saves the game to a chosen saved game slot
void Hex::saveGame()const
{
    cout<<endl<<"**********Save Game Menu**********"<<endl;

    int savedGameSlotNumber;
    string fileName;
    getSavedGames();
    cout<<"Saved game slots above are found. You can choose one of them to overwrite or choose a new save game slot."<<endl<<
    "To overwrite, enter the number of the existing saved game slot."<<endl<<
    "To create a new save game slot, enter a non existing saved game slot number between 1 and 20."<<endl<<
    "You can save up to 20 games maximum."<<endl;
    cin>>savedGameSlotNumber;
    cin.ignore(1000, '\n');
    while(savedGameSlotNumber<1 || savedGameSlotNumber>20)
    {
        if(!cin)
        {
           cin.clear();
           cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid game slot number, please enter again."<<endl;
        cin>>savedGameSlotNumber;
        cin.ignore(1000, '\n');
    }

    fileName="SavedGame"+to_string(savedGameSlotNumber)+".txt";//Opens a saved game slot with the chosen number
    ofstream writeGame(fileName);
    if(writeGame.fail())
    {
        cerr<<"File cannot be opened."<<endl;
        exit(-1);
    }

    //Stores the game related info
    writeGame<<getBoardHeight()<<endl<<getBoardWidth()<<endl<<getGameType()<<endl<<getWhoseTurn()<<endl<<
    getAllMarkedCells()<<endl<<getXCount()<<endl<<getOCount()<<endl;
    for(int i=0; i<getBoardHeight(); ++i)
        for(int j=0; j<getBoardWidth(); ++j)
            writeGame<<static_cast<char>(hexCells[i][j].getCellState());

    writeGame.close();

    cout<<"**********************************"<<endl;
    cout<<endl;
}

//Loads the game from a chosen saved game slot
void Hex::loadGame()
{
    cout<<endl<<"**********Load Game Menu**********"<<endl;

    int savedGameSlotNumber;
    string fileName;
    getSavedGames();
    for(int i=0; i<20; ++i)
    {
        if(fullSavedGameSlots[i]!=-1)
            break;
        else if(i==19)
        {
            cerr<<"There are no slots to load a game from."<<endl;
            exit(-1);
        }
    }
    cout<<"Saved game slots are found. You can choose one of them to load a game from."<<endl<<
    "To choose, enter the number of the existing saved game slot."<<endl;
    cin>>savedGameSlotNumber;
    cin.ignore(1000, '\n');
    for(int i=0; i<20; ++i)
    {
        if(getFullSavedGameSlots()[i]==savedGameSlotNumber)//Checks if the chosen slot has a game saved inside
            break;
        else if(i==19)
        {
            if(!cin)
            {
               cin.clear();
               cin.ignore(1000,'\n');
            }
            cout<<"You have entered an invalid saved game slot. Please enter again."<<endl;
            cin>>savedGameSlotNumber;
            cin.ignore(1000, '\n');
            i=-1;
        }
    }

    fileName="SavedGame"+to_string(savedGameSlotNumber)+".txt";//Opens the saved game slot with the given number
    ifstream readGame(fileName);
    if(readGame.fail())
    {
        cerr<<"File cannot be opened."<<endl;
        exit(-1);
    }

    //Registers the game related info from the chosen saved game slot
    int markedCellTemp;
    allMarkedCells-=(getXCount()+getOCount());
    readGame>>height>>width;
    readGame.ignore(1000, '\n');
    getline(readGame, gameType);
    readGame>>whoseTurn>>markedCellTemp>>xCount>>oCount;
    readGame.ignore(1000, '\n');
    allMarkedCells+=markedCellTemp;
    char stateTemp;

    hexCells.resize(getBoardHeight());//Resizes the board according to the new height and width
    for(int i=0; i<getBoardHeight(); ++i)
        hexCells[i].resize(getBoardWidth());

    for(int i=0; i<getBoardHeight(); ++i)
        for(int j=0; j<getBoardWidth(); ++j)
        {
            readGame>>stateTemp;
            hexCells[i][j].updateCellState(stateTemp);
        }

    readGame.close();
    cout<<endl<<"**********************************"<<endl;
    cout<<endl;
}

//Plays the game for a one move and passes the turn to the opponnent
void Hex::play(int chosenRow, char chosenColumn)
{
    if(whoseTurn=="User1" || whoseTurn=="User")
    {
        (hexCells[chosenRow-1][(static_cast<int>(chosenColumn))-65]).updateCellState('X');
        lastChosenCell.setCell(chosenRow, chosenColumn, CellState::User1orUser);
        //The move is registered as the lastChosenCell because it might be used by the computer when the computer is making a move
        cout<<"The move was "<<lastChosenCell.getCellColumn()<<lastChosenCell.getCellRow()<<endl;
        whoseTurn="User2";
        xCount++;
        allMarkedCells++;
    }
    else if(whoseTurn=="User2")
    {
        (hexCells[chosenRow-1][(static_cast<int>(chosenColumn))-65]).updateCellState('O');
        lastChosenCell.setCell(chosenRow, chosenColumn, CellState::User2orComputer);
        cout<<"The move was "<<lastChosenCell.getCellColumn()<<lastChosenCell.getCellRow()<<endl;
        whoseTurn="User1";
        oCount++;
        allMarkedCells++;
    }
}

//Plays the game for one move for computer and passes the turn to User
void Hex::play()
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
        if(doneMoves==0 && (lastChosenCell.getNeighbourCell(j)).getNeighbourCellRow()>0 && (lastChosenCell.getNeighbourCell(j)).getNeighbourCellRow()<=height &&
          (lastChosenCell.getNeighbourCell(j)).getNeighbourCellColumn()>='A' && (lastChosenCell.getNeighbourCell(j)).getNeighbourCellColumn()<=static_cast<char>(width+64) &&
          hexCells[(lastChosenCell.getNeighbourCell(j)).getNeighbourCellRow()-1][static_cast<char>((lastChosenCell.getNeighbourCell(j)).getNeighbourCellColumn())-65].getCellState()==CellState::Empty)
        {
            hexCells[(lastChosenCell.getNeighbourCell(j)).getNeighbourCellRow()-1][static_cast<char>((lastChosenCell.getNeighbourCell(j)).getNeighbourCellColumn())-65].updateCellState('O');
            displayBoard();
            cout<<endl<<"Computer move was "<<(lastChosenCell.getNeighbourCell(j)).getNeighbourCellColumn()<<(lastChosenCell.getNeighbourCell(j)).getNeighbourCellRow()<<endl;
            doneMoves++;
            oCount++;
            allMarkedCells++;
        }
        else if(i==6 && doneMoves==0)//If there is no neighbours to choose from, makes its move to the first empty cell that it finds
        {
           int j;
           for(int i=0; i<width; ++i)
           {
               for(j=0; j<height; ++j)
               {
                   if(hexCells[j][i].getCellState()==CellState::Empty)
                   {
                       hexCells[j][i].updateCellState('O');
                       displayBoard();
                       cout<<"Computer move was "<<static_cast<char>(j+65)<<i+1<<endl;
                       break;
                   }
               }
               if(hexCells[j][i].getCellState()==CellState::User2orComputer)
                   break;
           }
        }
    }
    whoseTurn="User";
}

//Checks the validity of the move input from the user
void Hex::controlCell(int& chosenRow, char& chosenColumn)
{
    bool validCheck=false;

    for(int i=0; i<height; ++i)
        for(int j=0; j<width; ++j)
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
    cout<<endl<<whoseTurn<<"'s Turn"<<endl;
    cout<<"Make your move: "<<endl;
    getline(cin, command);

    //If input command is not one of these, it means it is either an invalid one or a cell input. Converts the command to a proper cell input.
    if(command!="SAVE FILE.TXT" && command!="LOAD FILE.TXT" && command!="MENU" && command!="COMPARE")
    {
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
        chosenCell.setCell(nInput, cInput);
        controlCell(nInput, cInput);
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
        cout<<"There is not any other games to compare the current game. You game will continue from where it was left."<<endl;
        command="Continue";
    }
}

/*Compares two existing games. If the current game has more occupied cells for whoever has the turn (user1 or user2), the first game wins over the other game.
If one of the game's game type is user vs computer, for that game the user's occupied cells will be counted*/
bool Hex::compareGames(const Hex& otherGame)const
{
    int count1=0;
    int count2=0;

    if((getWhoseTurn()=="User1" || getWhoseTurn()=="User") && getXCount()>otherGame.getXCount())
        return true;
    else if(getWhoseTurn()=="User2" && otherGame.getGameType()=="user vs user" && getOCount()>otherGame.getOCount())
        return true;
    else if(getWhoseTurn()=="User2" && otherGame.getGameType()=="user vs computer" && getOCount()>otherGame.getXCount())
        return true;

    return false;
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
    while(Hex::currentGameNo<1 || Hex::currentGameNo>Hex::gameCount)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid number. Please enter again."<<endl;
        cin>>Hex::currentGameNo;
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

    if(whoseTurn=="Start" && gameType=="user vs user")
        whoseTurn="User1";
    else if(whoseTurn=="Start" && gameType=="user vs computer")
        whoseTurn="User";

    setCommand();
    /*It needs to exit from getting commands from the users if any of the users enters MENU or COMPARE commands because those commands
    might change the game's flow*/
    if(gameType=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE")
    {
        play(chosenCell.getCellRow(), chosenCell.getCellColumn());
        if(winCheck()==false)//It also checks if there is a win condition after each move
        {
           setCommand();
           if(gameType=="user vs user" && getCommand()!="MENU" && getCommand()!="COMPARE")
              play(chosenCell.getCellRow(), chosenCell.getCellColumn());
        }
    }
    if(gameType=="user vs computer" && getCommand()!="MENU" && getCommand()!="COMPARE")
    {
       play(chosenCell.getCellRow(), chosenCell.getCellColumn());
       if(winCheck()==false)
        play();
    }

    if(winCheck() && gameType=="user vs user" || (gameType=="user vs computer" && whoseTurn!="User"))
    {
        displayBoard();
        cout<<endl<<"You have won."<<endl;
    }
    else if(winCheck() && gameType=="user vs computer" && whoseTurn=="User")
        cout<<endl<<"Computer has won."<<endl;
}

//Checks if there is a win condition for any of the users or computer. Return true if there is
bool Hex::winCheck()const
{
    int pastCount=0;
    vector <Cell> pastCells;
    //Past cells is to store the cells the function has already checked before. The function should not visit them again
    pastCells.resize(width*height);

    for(int i=0; i<height; ++i)//Searchs for a cell to start checking from on the very left side
        if(hexCells[i][0].getCellState()==CellState::User1orUser)
        {
            pastCells[0].setCell(i+1, 'A');
            pastCount++;
            if(neighbourCheck(i+1, 'A', CellState::User1orUser, pastCount, pastCells))//Checks for a path of connected X that leads to a win condition
                return true;
            pastCount=0;
        }

    for(int j=0; j<width; ++j)//Searchs for a cell to start checking from at the top
        if(hexCells[0][j].getCellState()==CellState::User2orComputer)
        {
            pastCells[0].setCell(1, 'A'+j);
            pastCount++;
            if(neighbourCheck(1, 'A'+j, CellState::User2orComputer, pastCount, pastCells))//Checks for a path of connected O that leads to a win condition
                return true;
            pastCount=0;
        }

    return false;
}

//Used in winCheck. Checks if there is a path that leads to a win condition. Returns true if there is
bool Hex::neighbourCheck(int startRow, char startColumn, CellState stateInfo, int& pastCount, vector<Cell> &pastCells)const
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
       hexCells[(startCell.getNeighbourCell(j)).getNeighbourCellRow()-1][static_cast<char>((startCell.getNeighbourCell(j)).getNeighbourCellColumn())-65].getCellState()==stateInfo)
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