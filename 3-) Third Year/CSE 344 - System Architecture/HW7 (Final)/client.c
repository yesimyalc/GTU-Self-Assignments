#include "client.h"

int portNo=-1;  //Server port No
char* serverIP=NULL;    //Server IP
int requestCount=0; //Total requests count retrieved from the request file
pthread_mutex_t critMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t arrivedCond=PTHREAD_COND_INITIALIZER;
int arrived=0;  //thread count that arrived to the meeting barrier

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Checking the command line arguments
    checkArgc(argc);

    //Retrieving the command line inputs
    char* requestFilePath=NULL;
    retrieveArguments(argc, argv, &requestFilePath, &portNo, &serverIP);
    if(requestFilePath==NULL || portNo==-1 || serverIP==NULL)
    {
        char errorMessage[]="Arguments are not provided properly.\nYou need exactly 7 command line arguments (first being auto inputed) to run the program.\n-r for Request File, -q for Port Number, -s for Server IP.\nPlease try again.\n\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Read and store the requests
    //Opening the requestsFile
    int requestFile;
    requestFile=openFile(requestFilePath, O_RDONLY, 0555);
    if(requestFile<0)
    {
        perror("Cannot open the Request File in client");
        exit(EXIT_FAILURE);
    }

    //Reading the requests
    struct request* allRequests=NULL;
    char* readLine=NULL;
    int lineSize=0;
    while(true)
    {
        char readChar[1];
        int bytesread;
        while(((bytesread=read(requestFile, readChar, 1))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the request file in client");
            freePointer(allRequests);
            freePointer(readLine);
            close(requestFile);
            exit(EXIT_FAILURE);
        }
        else if(bytesread==0)
            break;
        else if(readChar[0]!='\n' && readChar[0]!='\r')
        {
            readLine=addCharacterToLine(readChar[0], &readLine, &lineSize);
            if(readLine==NULL)
            {
                writeOutput("Cannot allocate memory for readLine in client", STDERR_FILENO);
                freePointer(allRequests);
                freePointer(readLine);
                close(requestFile);
                exit(EXIT_FAILURE);
            }
        }
        else if(readChar[0]=='\n' && readLine!=NULL && isAllSpace(readLine)!=0)
        {
            struct request currentRequest;
            strcpy(currentRequest.city, "NoNe");
            retrieveRequestDataFromFile(readLine, &currentRequest);
            allRequests=addRequest(currentRequest, &allRequests, &requestCount);
            if(allRequests==NULL)
            {
                writeOutput("Cannot allocate memory for requests array in client", STDERR_FILENO);
                freePointer(readLine);
                close(requestFile);
                exit(EXIT_FAILURE);
            }
            freePointer(readLine);
            readLine=NULL;
            lineSize=0;
        }
    }

    freePointer(readLine);
    close(requestFile);

    //Create threads and direct the requests to threads
    //Start Output
    char output[3000];
    arrangeStartOutput(output, requestCount);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the start output of the client");

    pthread_t threads[requestCount];
    struct requiredData threadData[requestCount];
    for(int i=0; i<requestCount; ++i)
    {
        threadData[i].threadNo=i;
        threadData[i].threadRequest=allRequests[i];
        int returnVal=pthread_create(&threads[i] , NULL, threadFunction, &(threadData[i]));
        if(returnVal!=0)
        {
            perror("Failed to create client thread\n");
            freePointer(allRequests);
            for(int j=0; j<requestCount; ++j)
                pthread_join(threads[j], NULL);
            exit(EXIT_FAILURE);
        }
    }

    //Waiting for the threads
    for(int i=0; i<requestCount; ++i)
        pthread_join(threads[i], NULL);

    //Ending Output
    arrangeEndOutput(output);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the end output of the client");

    freePointer(allRequests);

    return 0;
}

void checkArgc(int count)
{
    if(count!=7)
    {
        char errorMessage[]="You need exactly 7 command line arguments (first being auto inputed) to run the program.\n-r for Request File, -q for Port Number, -s for Server IP.\nPlease try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], char** path, int* no, char** ip)
{
    int option;
    while((option=getopt(count, arguements, "r:q:s:"))!=-1)
        switch(option)
        {
            case 'r':
                *path=optarg;
                break;
            case 'q':
                *no=atoi(optarg);
                break;
            case 's':
                *ip=optarg;
                break;
            default:
                return -1;
        }

    return 0;
}

void retrieveRequestDataFromFile(char* requestStringG, struct request* newRequest)
{
    char* word;
    int counter=0;
    char requestString[strlen(requestStringG)];
    strcpy(requestString, requestStringG);
    char* buffer=requestString;

    while((word=strtok_r(buffer," ",&buffer)) !=NULL)
    {
        if(counter==1)
            strcpy(newRequest->type, word);
        else if(counter==2)
            strcpy(newRequest->startDate, word);
        else if(counter==3)
            strcpy(newRequest->endDate, word);
        else if(counter==4)
            strcpy(newRequest->city, word);
        else if(counter!=0)
            break;

        counter++;
    }
}

struct request* addRequest(struct request currentRequest, struct request** allRequests, int *requestCount)
{
    if(*requestCount==0)
    {
        *allRequests=(struct request*)malloc(sizeof(struct request));
        if(allRequests==NULL)
            return NULL;
        (*allRequests)[0]=currentRequest;
        (*requestCount)++;
        return *allRequests;
    }

    struct request* backupArray=(struct request*)malloc((*requestCount)*sizeof(struct request));
    if(backupArray==NULL)
        return NULL;

    for(int i=0; i<*requestCount; ++i)
        backupArray[i]=(*allRequests)[i];

    *allRequests=(struct request*)realloc(*allRequests, (*requestCount)*sizeof(struct request)+sizeof(struct request));
    if(*allRequests==NULL)
    {
        free(backupArray);
        return(NULL);
    }
    for(int i=0; i<*requestCount; ++i)
        (*allRequests)[i]=backupArray[i];
    (*allRequests)[*requestCount]=currentRequest;

    (*requestCount)++;

    free(backupArray);

    return *allRequests;
}

void arrangeStartOutput(char* output, int requestC)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Client: I have loaded %d requests and I'm creating %d threads.\n", requestC, requestC);
    strcat(output, buffer);
}

void* threadFunction(void* args)
{
    //Start Output
    char output[3000];
    arrangeThreadStartOutput(output, ((struct requiredData*)args)->threadNo);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the start output of the client thread");

    //Synchronization Barrier
    int returnVal=0;
    returnVal=pthread_mutex_lock(&critMutex);
    if(returnVal!=0)
    {
        perror("Cannot lock mutex in client thread");
        arrived++;
        pthread_cond_broadcast(&arrivedCond);
        return NULL;
    }

    arrived++;

    while(arrived<requestCount)
    {
        pthread_cond_wait(&arrivedCond, &critMutex);
        if(returnVal!=0)
        {
            perror("Cannot wait on condition in client thread");
            pthread_cond_broadcast(&arrivedCond);
            pthread_mutex_unlock(&critMutex);
            return NULL;
        }
    }

    pthread_cond_broadcast(&arrivedCond);
    pthread_mutex_unlock(&critMutex);

    //Request Output
    arrangeRequestOutput(output, ((struct requiredData*)args)->threadNo, ((struct requiredData*)args)->threadRequest);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the request output of the client thread");

    //Server Connection
    int serverSocket;
    int counter=0;
    struct sockaddr_in serv_addr;
    while(((serverSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
    if(serverSocket<0)
    {
        perror("Failed to create socket");
        return NULL;
    }
    counter=0;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portNo);
    serv_addr.sin_addr.s_addr=inet_addr(serverIP);

    while((returnVal=connect(serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Failed to connect");
        close(serverSocket);
        return NULL;
    }
    counter=0;

    char* socketInput=arrangeClientSocketInput(&socketInput, *((struct request*)args));
    if(socketInput==NULL)
    {
        perror("Cannot allocate memory for socket input in client");
        close(serverSocket);
        return NULL;
    }

    //Writing to socket
    while(((returnVal=write(serverSocket, socketInput, strlen(socketInput)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Failed to write to the socket in client");
        free(socketInput);
        close(serverSocket);
        return NULL;
    }

    //Waiting for response
    int transactionCount=0;
    while(((returnVal=read(serverSocket, &transactionCount, sizeof(int)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Failed to read from the socket in client");
        free(socketInput);
        close(serverSocket);
        return NULL;
    }

    //Response Output
    arrangeResponseOutput(output, ((struct requiredData*)args)->threadNo, ((struct requiredData*)args)->threadRequest, transactionCount);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the response output of the client thread");

    //Exit Output
    arrangeThreadEndOutput(output, ((struct requiredData*)args)->threadNo);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the end output of the client thread");

    close(serverSocket);
    free(socketInput);

    return NULL;
}

void arrangeThreadStartOutput(char* output, int threadNo)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Client-Thread-%d: Thread-%d has been created\n", threadNo, threadNo);
    strcat(output, buffer);
}

void arrangeRequestOutput(char* output, int threadNo, struct request threadRequest)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    if(strcmp(threadRequest.city, "NoNe")!=0)
        snprintf(buffer, 3000, ": Client-Thread-%d: I am requesting \"transactionCount %s %s %s %s\"\n", threadNo, threadRequest.type, threadRequest.startDate, threadRequest.endDate, threadRequest.city);
    else
        snprintf(buffer, 3000, ": Client-Thread-%d: I am requesting \"transactionCount %s %s %s\"\n", threadNo, threadRequest.type, threadRequest.startDate, threadRequest.endDate);

    strcat(output, buffer);
}

void arrangeResponseOutput(char* output, int threadNo, struct request threadRequest, int result)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    if(strcmp(threadRequest.city, "NoNe")!=0 && result!=-1)
        snprintf(buffer, 3000, ": Client-Thread-%d: The server's response to \"transactionCount %s %s %s %s\" is %d\n", threadNo, threadRequest.type, threadRequest.startDate, threadRequest.endDate, threadRequest.city, result);
    else if(result!=-1)
        snprintf(buffer, 3000, ": Client-Thread-%d: The server's response to \"transactionCount %s %s %s\" is %d\n", threadNo, threadRequest.type, threadRequest.startDate, threadRequest.endDate, result);
    else
       snprintf(buffer, 3000, ": Client-Thread-%d: The city given cannot be found in the dataset\n", threadNo);

    strcat(output, buffer);
}

void arrangeThreadEndOutput(char* output, int threadNo)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Client-Thread-%d: Terminating\n", threadNo);
    strcat(output, buffer);
}

void arrangeEndOutput(char* output)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Client: All threads have terminated, goodbye.\n");
    strcat(output, buffer);
}

int isAllSpace(char* line)
{
    for(int i=0; i<strlen(line); ++i)
        if(line[i]!=' ' && line[i]!='\t')
            return -1;

    return 0;
}
