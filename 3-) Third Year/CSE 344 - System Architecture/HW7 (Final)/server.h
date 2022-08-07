#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "clientServerCommunication.h"

//requestQueue structure is used to represent the request queue in the server
//In this queue retrieved file descriptors from connections with clients are held
//totalRequests is used to check if the queue is full, if the queue needs to go back to the start (circular)
//currentIndex is used by threads to find out the next waiting file descriptor
//active requests is used to check if there is any waiting requests
struct requestQueue{
    int clientSocketFdArray[1024];
    int totalRequests;
    int currentIndex;
    int activeRequests;
};

//Retrieves the required information from the arguments.
//Returns 0 on success, -1 on failure
//The retrieved info are assigned to portNo, tCount
int retrieveArguments(int count, char* arguements[], int* portNo, int* tCount);

//Retrieves servant's data from the given dataString
//Puts the retrieved information into given newServant structure
void retrieveServantData(char* dataString, struct servantInfo* newServant);

//Adds a new servantInfo to a dynamic servantInfo array.
//Returns the pointer on success, NULL on failure
//Increments the size of the string given in 3rd parameter
//Assigns the new servantInfo to the second parameter as well
struct servantInfo* addServant(struct servantInfo currentServant, struct servantInfo** allServants, int *servantCount);

//Thread function of server. Waits for a request, retrieves when available and deals with it.
//The args include the thread number of the thread
void* threadFunction(void* args);

//Formats the request arrived output of server
//The output is put inside the first parameter
//threadNo is the number of the printing thread
//no is the thread no
void arrangeRequestArrivedOutput(char* output, struct request arrivedRequest, int no);

//Formats the servant connection output of server
//The output is put inside the first parameter
//threadNo is the number of the printing thread
//no is the thread no
//servantP is the connecting servant PID
void arrangeServantConnectionOutput(char* output, int servantP, int no);

//Formats the result retrieved output of server
//The output is put inside the first parameter
//threadNo is the number of the printing thread
//no is the thread no
//result is the retrieved result for a request
void arrangeResultRetrieved(char* output, int result, int no);

//Formats the servant loaded output of server
//The output is put inside the first parameter
//threadNo is the number of the printing thread
//loadedServant is a structure containing information related to the recently loaded servant
void arrangeServantLoadedOutput(char* output, struct servantInfo loadedServant);

//When a SIGINT is received, this handler is run.
//It assigns the sigintFlag as 1
void sigintHandler(int signum);

//Formats termination output of server
//The output is put inside the first parameter
//threadNo is the number of the printing thread
void arrangeTerminationOutput(char* output);

#endif // SERVER_H_INCLUDED
