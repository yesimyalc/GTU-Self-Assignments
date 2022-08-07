#ifndef CLIENTSERVERCOMMUNICATION_H_INCLUDED
#define CLIENTSERVERCOMMUNICATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static const char default_format[] = "%b %d %Z %Y %H %M %S";

//request structure is used to store request information coming from clients
struct request{
    char type[500];
    char startDate[500];
    char endDate[500];
    char city[500];
};

//cityInfo structure is used to store information related to a city.
struct cityInfo{
    int cityNo;
    char cityName[500];
};

//servantInfo structure is used to store information related to a servant
//This servantPort is the port number assigned to the servant
//All information related to the servant's assigned cities are held in cities array
struct servantInfo{
    int servantPID;
    int servantPort;
    int cityCount;
    struct cityInfo* cities;
};

/*package structure is used to store information sent by a socket.
Socket data sending is done with 1 line strings then the string is analyzed to
fill a package.
isClient is 1 if the data came from a client, 0 if came from a servant
if the data came from a client, clientRequest is filled, otherwise servant is filled*/
struct package{
    int isClient;
    struct request clientRequest;
    struct servantInfo servant;
};

//transactions structure is used to store information related to transaction of a city
struct transactions
{
    char id[500];
    char date[500];
    char type[500];
    char street[500];
    char surface[500];
    char price[500];
};

/* node structure is used in the AVL tree. The node belongs to a city and
holds it's all information.
left and right pointers are for traversal in the tree.
height is used to balance the tree*/
struct node
{
    char cityName[500];
    int transactionCount;
    struct transactions* cityTransactions;

    struct node* left;
    struct node* right;
    int height;
};

//Checks if given argument count is enough
void checkArgc(int count);

//Adds a new character to a dynamic string.
//Returns the pointer on success, NULL on failure
//Increments the size of the string given in 3rd parameter
//Assigns the new string to the second parameter as well
char* addCharacterToLine(char newCharacter, char** oldLine, int* oldLineSize);

//Opens a file with the given flags and modes.
//Protected against signal interrupts
//Returns -1 on failure, the fd of the file on success
int openFile(char* filePath, int flags, mode_t mode);

//Writes the given output to given fileFd
//Protected against signal interrupts
//Returns -1 on failure, 0 on success
int writeOutput(char* output, int fileFd);

//Frees a pointer if it is not NULL
void freePointer(void* pointer);

//Adds a new city to a dynamic cityInfo array.
//Returns the pointer on success, NULL on failure
//Increments the size of the string given in 3rd parameter
//Assigns the new cityInfo to the second parameter as well
struct cityInfo* addCity(struct cityInfo currentCity, struct cityInfo** allCities, int *cityCount);

//Breaks the given requestString into parts and stores it's information in the given newRequest
void retrieveRequestData(char* requestStringG, struct request* newRequest);

//Arranges the given currentRequest in a one line string format.
//Returns the pointer on success, NULL on failure
//Also assigns the new input to the first parameter
char* arrangeClientSocketInput(char** socketInput, struct request currentRequest);

//Inserts a new node to the AVL tree
//The node is initialized with the given city name, count for transaction amount and a transaction array
struct node* insert(struct node * binary_tree, char city[500], int count, struct transactions* tran);

//Searches in AVL Tree with a given city name
//If found, returns the city node, if not returns NULL
struct node *search(struct node ** binary_tree, char city[500]);

//Deletes the given tree and frees it's resources
void delete_tree(struct node * binary_tree);

#endif // CLIENTSERVERCOMMUNICATION_H_INCLUDED
