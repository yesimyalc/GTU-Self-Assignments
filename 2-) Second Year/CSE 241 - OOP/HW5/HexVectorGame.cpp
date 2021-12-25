#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "cell.h"
#include "exceptions.h"
#include "AbstractHex.h"
#include "HexVector.h"

using namespace std;
using namespace HexGame_200104004094;

int main()
{
    cout<<"***************Welcome to Hex Game***************"<<endl<<
    endl<<"*****Game Rules*****"<<endl<<
    "-If you are playing the game with two users, User1 is using 'X' and User2 is using 'O'"<<endl<<
    "-If you are playing the game with computer, User is using 'X' and Computer is using 'O'"<<endl<<
    "-User1 or User needs to connect very left and very right sides, User2 or Computer needs to connect top and bottom."<<endl<<
    "-You can connect cells by moving to east, west, northeast, northwest, southeast or southwest."<<endl<<
    endl<<"*****How to Enter Commands*****"<<endl<<
    "-You can determine your move by entering cell coordinates. (example: F1, B1, A5)"<<endl<<
    "-If you want to save or load the game at any point,"<<endl<<
    "you can use LOAD and SAVE commands when you are asked to make your move."<<endl<<
    "-To reset a game, you can use RESET command."<<endl<<
    "-To exit from all games, you can use EXIT command."<<endl<<
    "-To edit the game, you can enter CHANGE command."<<endl<<
    "To see the last move made, you can use LAST command."<<endl<<
    "-To compare the current game with another game, you can use COMPARE command when you are asked to make your move."<<endl;
    cout<<"**************************************************"<<endl<<endl;

    int i;

    cout<<"How many games do you want to create?"<<endl;
    cin>>AbstractHex::gameCount;
    cin.ignore(1000,'\n');
    while(AbstractHex::gameCount<1)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid number. Please enter again."<<endl;
        cin>>AbstractHex::gameCount;
        cin.ignore(1000,'\n');
    }

    vector<AbstractHex*> hexGames;
    hexGames.resize(AbstractHex::gameCount);//Opens hex games according to how many the user wants to open with the default constructor I created
    for(int i=0; i<AbstractHex::gameCount; ++i)
        hexGames[i]=new HexVector;

    vector<string> gameStatus;
    gameStatus.resize(AbstractHex::gameCount);
    for(i=0; i<AbstractHex::gameCount; ++i)//The user can choose to open an old game which was saved before
    {
        cout<<"Enter if the game will be a new game or an old game for game "<<i+1<<" : (new game or old game)"<<endl;
        getline(cin, gameStatus[i]);
        while(gameStatus[i]!="new game" && gameStatus[i]!="old game")
        {
            cout<<"You have entered an invalid game status. Please enter again."<<endl;
            getline(cin, gameStatus[i]);
        }
        if(gameStatus[i]=="old game")
        {
            cout<<endl<<"**********Load Game Menu**********"<<endl;

            cout<<"Enter the saved game file you want to open. Do not forget to include .txt or .TXT in the file name."<<endl;
            string fileName;
            getline(cin, fileName);

            try{
            hexGames[i]->readFromFile(fileName);
            }
            catch(CannotOpenFileException& exc)
            {
                cerr<<exc.what()<<endl;
                exit(-1);
            }
        }
    }

    cout<<endl<<"Which game do you want to start?"<<endl;
    for(i=0; i<AbstractHex::gameCount; ++i)
        cout<<i+1<<"-) Game "<<i+1<<endl;
    cin>>AbstractHex::currentGameNo;
    cin.ignore(1000,'\n');
    while(AbstractHex::currentGameNo<1 || AbstractHex::currentGameNo>AbstractHex::gameCount)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid number. Please enter again."<<endl;
        cin>>AbstractHex::currentGameNo;
        cin.ignore(1000,'\n');
    }

    cout<<endl;
    cout<<"Game "<<AbstractHex::currentGameNo<<" is being started."<<endl;

    while(AbstractHex::currentGameNo!=-1 && hexGames[AbstractHex::currentGameNo-1]->getCommand()!="EXIT")
    {
        hexGames[AbstractHex::currentGameNo-1]->playGame();

        if(hexGames[AbstractHex::currentGameNo-1]->getCommand()=="EXIT")
            break;

        //If compare command is given, the game will continue right after the comparison finishes.
        if(hexGames[AbstractHex::currentGameNo-1]->getCommand()=="COMPARE")
        {
            cout<<endl<<"**********Compare Menu**********"<<endl<<endl;
            cout<<"You can compare your current game with any of the games you choose."<<endl<<
            "If the game boards are the same, it means the games are equal."<<endl;

            int compare;
            cout<<endl<<"There are "<<AbstractHex::gameCount<<" games opened. Which one do you want to compare your current game with? Do not choose the current game number"<<endl<<
            AbstractHex::currentGameNo<<"-) Current Game"<<endl;
            for(int i=0; i<AbstractHex::gameCount; ++i)
               if(i+1!=AbstractHex::currentGameNo)
                  cout<<i+1<<"-) Game "<<i+1<<endl;
            cin>>compare;
            cin.ignore(1000,'\n');
            while(compare<1 || compare>AbstractHex::gameCount || compare==AbstractHex::currentGameNo)
            {
                if(!cin)
               {
                  cin.clear();
                  cin.ignore(1000,'\n');
               }
               cout<<"You have entered an invalid game number. Please enter again."<<endl;
               cin>>compare;
               cin.ignore(1000,'\n');
            }

            if((*hexGames[AbstractHex::currentGameNo-1])==(*hexGames[compare-1]))
                cout<<"The games are equal"<<endl;
            else
                cout<<"The games are not equal"<<endl;
            cout<<endl<<"********************************"<<endl;
        }

        for(i=0; i<AbstractHex::gameCount; ++i)//Checks if all the games have been finished
            if(hexGames[i]->isEnd()==false)
               break;
        if(i==AbstractHex::gameCount)//Breaks the loop to exit if all of them have been finished.
        {
            cout<<endl<<"All games are finished."<<endl;
            break;
        }

        //If the current game has been finished, the program will make the user switch to another existing game
        if(hexGames[AbstractHex::currentGameNo-1]->isEnd()==true)
        {
            cout<<endl<<"The current game has ended. There are "<<AbstractHex::gameCount-1<<" more games."<<endl<<
            AbstractHex::currentGameNo<<"-) Current Game"<<endl;
            for(i=0; i<AbstractHex::gameCount; ++i)
                if(i+1!=AbstractHex::currentGameNo)
                cout<<i+1<<"-) Game "<<i+1<<endl;

            cout<<"If you want to continue any of them, enter their number. If you want to quit all enter -1."<<endl;
            cin>>AbstractHex::currentGameNo;
            cin.ignore(1000, '\n');
            while(AbstractHex::currentGameNo!=-1 && (AbstractHex::currentGameNo<1 || AbstractHex::currentGameNo>AbstractHex::gameCount || hexGames[AbstractHex::currentGameNo-1]->isEnd()==true))
            {
               if(!cin)
               {
                  cin.clear();
                  cin.ignore(1000,'\n');
               }
               cout<<"You have entered an invalid number or a finished game's number. Please enter again."<<endl;
               cin>>AbstractHex::currentGameNo;
               cin.ignore(1000,'\n');
            }
        }
    }

    for(int i=0; i<AbstractHex::gameCount; ++i)
        delete hexGames[i];

    return 0;
}
