#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include "Hex.h"

using namespace std;

int Hex::allMarkedCells=0;
vector<int> Hex::fullSavedGameSlots;
string Hex::command="Start";
int Hex::currentGameNo=0;
int Hex::gameCount=0;

int main()
{
    int i;

    cout<<"***************Welcome to Hex Game***************"<<endl<<
    endl<<"*****Game Rules*****"<<endl<<
    "-If you are playing the game with two users, User1 is using 'X' and User2 is using 'O'"<<endl<<
    "-If you are playing the game with computer, User is using 'X' and Computer is using 'O'"<<endl<<
    "-User1 or User needs to connect very left and very right sides, User2 or Computer needs to connect top and bottom."<<endl<<
    "-You can connect cells by moving to east, west, northeast, northwest, southeast or southwest."<<endl<<
    endl<<"*****How to Enter Commands*****"<<endl<<
    "-You can determine your move by entering cell coordinates. (example: F1, B1, A5)"<<endl<<
    "-If you want to save or load the game at any point,"<<endl<<
    "you can use LOAD FILE.TXT and SAVE FILE.TXT commands when you are asked to make your move."<<endl<<
    "-You can create more than one games at a time and can move between them."<<endl<<
    "-To move between games, you can use MENU command when you are asked to make your move."<<endl<<
    "-To compare the current game with another game, you can use COMPARE command when you are asked to make your move."<<endl<<endl;
    cout<<"**************************************************"<<endl<<endl;

    cout<<"How many games do you want to create? You can create minimum 5 games."<<endl;
    /*Because we are asked to create at least 5 hex games, I limited the user to open 5 or more games at first but if that was not wanted in the homework,
    I could have limited it to just one*/
    cin>>Hex::gameCount;
    cin.ignore(1000,'\n');
    while(Hex::gameCount<5)
    {
        if(!cin)
        {
            cin.clear();
            cin.ignore(1000,'\n');
        }
        cout<<"You have entered an invalid number. Please enter again."<<endl;
        cin>>Hex::gameCount;
        cin.ignore(1000,'\n');
    }

    vector<Hex> hexGames;
    hexGames.resize(Hex::gameCount);//Opens hex games according to how many the user wants to open with the default constructor I created

    vector<string> gameTypeInput;
    gameTypeInput.resize(Hex::gameCount);

    vector<string> gameStatus;
    gameStatus.resize(Hex::gameCount);
    for(i=0; i<Hex::gameCount; ++i)//The user can choose to open an old game which was saved before
    {
        cout<<"Enter if the game will be a new game or an old game for game "<<i+1<<" : (new game or old game)"<<endl;
        getline(cin, gameStatus[i]);
        while(gameStatus[i]!="new game" && gameStatus[i]!="old game")
        {
            cout<<"You have entered an invalid game status. Please enter again."<<endl;
            getline(cin, gameStatus[i]);
        }
        if(gameStatus[i]=="old game")
            hexGames[i].loadGame();
    }

    for(i=0; i<Hex::gameCount; ++i)//Resets the opened hex games according to the user input of game types
    {
        if(gameStatus[i]=="new game")
        {
            cout<<"Enter the game type of game "<<i+1<<" :(user vs user  or  user vs computer)"<<endl;
            getline(cin, gameTypeInput[i]);
            while(gameTypeInput[i]!="user vs user" && gameTypeInput[i]!="user vs computer")
            {
                cout<<"You have entered an invalid game type. Please enter again."<<endl;
                getline(cin, gameTypeInput[i]);
            }

            hexGames[i]=Hex(gameTypeInput[i]);
            //Decided to copy paste temporary constructed hex games to the opened games because I had to construct them before when I resized the hexGames vector
        }
    }

    cout<<endl<<"Which game do you want to start?"<<endl;
    for(i=0; i<Hex::gameCount; ++i)
        cout<<i+1<<"-) Game "<<i+1<<endl;
    cin>>Hex::currentGameNo;
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

    cout<<"Game "<<Hex::currentGameNo<<" is being started."<<endl;

    //This loop will keep the games being played until the player uses -1 command to exit the games or till all the games finishes
    while(Hex::currentGameNo!=-1)
    {
        hexGames[Hex::currentGameNo-1].playGame();

        //If compare command is given, the game will continue right after the comparison finishes.
        if(hexGames[Hex::currentGameNo-1].getCommand()=="COMPARE")
        {
            cout<<endl<<"**********Compare Menu**********"<<endl<<endl;
            cout<<"You can compare your current game with any of the games you choose."<<endl<<
            "If the current game has more occupied cells for whoever has the turn (user1 or user2), the first game wins over the other game."<<endl<<
            "Otherwise the second game wins over the first game."<<endl<<
            "If the two game types are different (one user vs user, other user vs computer),"<<endl<<
            "user cells will be counted from the game which is user vs computer."<<endl;
            int compare;
            cout<<endl<<"There are "<<Hex::gameCount<<" games opened. Which one do you want to compare your current game with? Do not choose the current game number"<<endl<<
            Hex::currentGameNo<<"-) Current Game"<<endl;
            for(int i=0; i<Hex::gameCount; ++i)
               if(i+1!=Hex::currentGameNo)
                  cout<<i+1<<"-) Game "<<i+1<<endl;
            cin>>compare;
            cin.ignore(1000,'\n');
            while(compare<1 || compare>Hex::gameCount || compare==Hex::currentGameNo)
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

            if(hexGames[Hex::currentGameNo-1].compareGames(hexGames[compare-1]))
                cout<<"The current game wins over the other game."<<endl;
            else
                cout<<"The other game wins over the current game or they are equal."<<endl;
            cout<<endl<<"********************************"<<endl;
        }

        for(i=0; i<Hex::gameCount; ++i)//Checks if all the games have been finished
            if(hexGames[i].winCheck()==false)
               break;
        if(i==Hex::gameCount)//Breaks the loop to exit if all of them have been finished.
        {
            cout<<endl<<"All games are finished."<<endl;
            break;
        }

        //If the current game has been finished, the program will make the user switch to another existing game
        if(hexGames[Hex::currentGameNo-1].winCheck()==true)
        {
            cout<<endl<<"The current game has ended. There are "<<Hex::gameCount-1<<" more games."<<endl<<
            Hex::currentGameNo<<"-) Current Game"<<endl;
            for(i=0; i<Hex::gameCount; ++i)
                if(i+1!=Hex::currentGameNo)
                cout<<i+1<<"-) Game "<<i+1<<endl;

            cout<<"If you want to continue any of them, enter their number. If you want to quit all enter -1."<<endl;
            cin>>Hex::currentGameNo;
            cin.ignore(1000, '\n');
            while(Hex::currentGameNo!=-1 && (Hex::currentGameNo<1 || Hex::currentGameNo>Hex::gameCount || hexGames[Hex::currentGameNo-1].winCheck()==true))
            {
               if(!cin)
               {
                  cin.clear();
                  cin.ignore(1000,'\n');
               }
               cout<<"You have entered an invalid number or a finished game's number. Please enter again."<<endl;
               cin>>Hex::currentGameNo;
               cin.ignore(1000,'\n');
            }
        }
    }

    return 0;
}


