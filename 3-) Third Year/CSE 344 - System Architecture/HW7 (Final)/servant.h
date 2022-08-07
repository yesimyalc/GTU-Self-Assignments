#ifndef SERVANT_H_INCLUDED
#define SERVANT_H_INCLUDED

#include "clientServerCommunication.h"

//Retrieves the required information from the arguments.
//Returns 0 on success, -1 on failure
//The retrieved info are assigned to path, start, end, ip, portNo
int retrieveArguments(int count, char* arguements[], char** path, int* start, int* end, char** ip, int* portNo);

//Retrieves the servant's assigned citys' numebrs from the given numbersString
//Assigns them to start and end
void retrieveCityNumbers(char* numbersString, int* start, int* end);

//Compares two city names.
//Returns a value >0 if a>b
//Returns 0 if a=b
//Returns a value <0 is a<b
int compareCityNames(const void * a, const void * b);

//Arranges the given currentservant in a one line string format.
//Returns the pointer on success, NULL on failure
//Also assigns the new input to the first parameter
char* arrangeSocketInput(char** socketInput, struct servantInfo currentservant);

//Formats the data loaded output of servant
//The output is put inside the first parameter
//pid is the pid of the servant
//The servant will be responsible of cities between firstCityName and secondCityName
void arrangeDataLoadedOutput(char* output, int servantPid, char* firstCityName, char* secondCityName);

//Formats the servant start output of servant
//The output is put inside the first parameter
//port is the port number of the server
//pid is the pid of the servant
void arrangeServantStartOutput(char* output, int pid, int port);

//Thread function of servant. Deals with the given connection.
//args contain a file descriptor for server socket
void* threadFunction(void* args);

//Adds a new transaction to a dynamic transaction array.
//Returns the pointer on success, NULL on failure
//Increments the size of the string given in 3rd parameter
//Assigns the new transaction to the second parameter as well
struct transactions* addTransaction(struct transactions currentTransaction, struct transactions** allTransactions, int *transactionCount);

//Checks if the given dateE lies in between startT and endD
//Returns 1 if true, 0 if false
int compareDates(char* endD, char* startT, char* dateE);

//Calculates if the date with given d1, m1, y1 is bigger than given d2, m2, y2
//If true, returns 1. If false, return -1
int calculateDateDifference(int d1, int m1, int y1, int d2, int m2, int y2);

//When a SIGINT is received, this handler is run.
//It assigns the sigintFlag as 1
void sigintHandler(int signum);

//Formats the termination output of servant
//The output is put inside the first parameter
//pid is the servant's pid
void arrangeTerminationOutput(char* output, int pid);

//Used during atexit
//Frees some globally used pointers
void exitFunction(void);


#endif // SERVANT_H_INCLUDED
