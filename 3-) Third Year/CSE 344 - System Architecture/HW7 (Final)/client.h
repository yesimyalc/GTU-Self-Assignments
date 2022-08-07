#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "clientServerCommunication.h"

//requiredData structure is used to pass information to a created thread.
//threadRequest is the request that the threads is assigned to
struct requiredData{
    struct request threadRequest;
    int threadNo;
};

//Retrieves the required information from the arguments.
//Returns 0 on success, -1 on failure
//The retrieved info are assigned to path, no and ip
//Path is the request file datapath, no is port no, ip is server ip
int retrieveArguments(int count, char* arguements[], char** path, int* no, char** ip);

//Retrieves the request data from request file
//Breaks apart the given requestString and inserts the information into newRequest structure
void retrieveRequestDataFromFile(char* requestStringG, struct request* newRequest);

//Adds a new request to a dynamic request array.
//Returns the pointer on success, NULL on failure
//Increments the size of the string given in 3rd parameter
//Assigns the new request to the second parameter as well
struct request* addRequest(struct request currentRequest, struct request** allRequests, int *requestCount);

//Formats the start output of the client
//The output is put inside the first parameter
//requestC is client's total request count
void arrangeStartOutput(char* output, int requestC);

//Thread function of client. Deals with it's assigned request
//args include a requiredData structure
void* threadFunction(void* args);

//Formats the thread start output of client
//The output is put inside the first parameter
//threadNo is the number of the printing thread
void arrangeThreadStartOutput(char* output, int threadNo);

//Formats the request output of client
//The output is put inside the first parameter
//threadNo is the number of the printing thread
//threadRequest is a structure containing the request information of the client
void arrangeRequestOutput(char* output, int threadNo, struct request threadRequest);

//Formats the response output of client
//The output is put inside the first parameter
//threadRequest is a structure containing the request information of the client
//result is the result for the request
void arrangeResponseOutput(char* output, int threadNo, struct request threadRequest, int result);

//Formats the thread end output of client
//The output is put inside the first parameter
//threadNo is the number of the printing thread
void arrangeThreadEndOutput(char* output, int threadNo);

//Formats the end output of client
//The output is put inside the first parameter
void arrangeEndOutput(char* output);

//Checks if a string given as the first parameter is only space
int isAllSpace(char* line);

#endif // CLIENT_H_INCLUDED
