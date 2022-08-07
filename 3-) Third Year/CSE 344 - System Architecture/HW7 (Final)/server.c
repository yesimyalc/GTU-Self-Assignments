#include "server.h"

struct requestQueue clientRequests; //queue structure for all retrieved client requests
struct servantInfo* allServants=NULL;   //an array which holds all servant's information port/pid
int servantCount=0; //count of servant that is connected to the server
int sigintFlag=0;   //a flag to check if sigint has been received
int handledRequestCount=0;  //total count of handled requests
pthread_mutex_t critMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueEmpty=PTHREAD_COND_INITIALIZER;
pthread_cond_t queueFull=PTHREAD_COND_INITIALIZER;

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Assigning signal handlers
    //SIGINT
    struct sigaction sigintAct={{0}};
    sigemptyset(&sigintAct.sa_mask);
    sigintAct.sa_handler=&sigintHandler;
    if(sigaction(SIGINT, &sigintAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        exit(EXIT_FAILURE);
    }

    //Checking the command line arguments
    checkArgc(argc);

    //Retrieving the command line inputs
    int threadCount=-1;
    int portNo=-1;
    retrieveArguments(argc, argv, &portNo, &threadCount);
    if(portNo==-1 || threadCount<5)
    {
        char errorMessage[]="Arguments are not provided properly.\nYou need exactly 5 command line arguments (first being auto inputed) to run the program.\n-p for port number, -t for thread count.\nPlease try again.\n\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Initializing request queue
    int returnVal=0;
    clientRequests.currentIndex=0;
    clientRequests.totalRequests=0;
    clientRequests.activeRequests=0;
    for(int i=0; i<1024; i++)
        clientRequests.clientSocketFdArray[i]=-1;

    //Creating threads
    pthread_t threads[threadCount];
    int threadNumbers[threadCount];
    for(int i=0; i<threadCount; ++i)
    {
        threadNumbers[i]=i;
        int returnVal=pthread_create(&threads[i] , NULL, threadFunction, &(threadNumbers[i]));
        if(returnVal!=0)
        {
            writeOutput("Failed to create server thread\n", STDERR_FILENO);
            sigintFlag=1;
            pthread_cond_broadcast(&queueEmpty);
            for(int j=0; j<i; ++j)
                pthread_join(threads[j], NULL);
            exit(EXIT_FAILURE);
        }
    }

    //Opening the server socket
    int serverSocket;
    struct sockaddr_in serv_addr;
    int counter=0;
    while(((serverSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
    if(serverSocket<0)
    {
        perror("Failed to create socket in server");
        sigintFlag=1;
        pthread_cond_broadcast(&queueEmpty);
        for(int i=0; i<threadCount; ++i)
            pthread_join(threads[i], NULL);
        exit(EXIT_FAILURE);
    }
    counter=0;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portNo);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    while((returnVal=bind(serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Bind failed in server");
        sigintFlag=1;
        pthread_cond_broadcast(&queueEmpty);
        for(int i=0; i<threadCount; ++i)
            pthread_join(threads[i], NULL);
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    counter=0;

    while((returnVal=listen(serverSocket, 1024))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Failed to listen in server");
        sigintFlag=1;
        pthread_cond_broadcast(&queueEmpty);
        for(int i=0; i<threadCount; ++i)
            pthread_join(threads[i], NULL);
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    counter=0;

    //Server connected waiting for requests
    while(true)
    {
        if(sigintFlag==1)
        {
            char output[1000];
            arrangeTerminationOutput(output);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the termination output of the server");

            pthread_cond_broadcast(&queueEmpty);
            for(int i=0; i<threadCount; ++i)
                pthread_join(threads[i], NULL);

            for(int i=0; i<servantCount; ++i)
                kill(allServants[i].servantPID, SIGINT);

            for(int i=clientRequests.currentIndex; i<1024; ++i)
                if(clientRequests.clientSocketFdArray[i]!=-1)
                    close(clientRequests.clientSocketFdArray[i]);

            for(int i=0; i<servantCount; ++i)
                freePointer(allServants[i].cities);
            freePointer(allServants);
            close(serverSocket);
            return 0;
        }

        int retrievedSocket;
        int addrlen=sizeof(serv_addr);
        while((retrievedSocket=accept(serverSocket, (struct sockaddr*)&serv_addr, (socklen_t*)&addrlen))<0 && errno==EINTR && sigintFlag==0);
        if(sigintFlag==1)
        {
            char output[1000];
            arrangeTerminationOutput(output);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the termination output of the server");

            pthread_cond_broadcast(&queueEmpty);
            for(int i=0; i<threadCount; ++i)
                pthread_join(threads[i], NULL);

            for(int i=clientRequests.currentIndex; i<1024; ++i)
                if(clientRequests.clientSocketFdArray[i]!=-1)
                    close(clientRequests.clientSocketFdArray[i]);

            for(int i=0; i<servantCount; ++i)
                kill(allServants[i].servantPID, SIGINT);

            for(int i=0; i<servantCount; ++i)
                freePointer(allServants[i].cities);
            freePointer(allServants);
            close(serverSocket);
            return 0;
        }

        if(retrievedSocket<0)
        {
            perror("Failed to accept in server");
            sigintFlag=1;
            pthread_cond_broadcast(&queueEmpty);
            for(int i=0; i<threadCount; ++i)
                pthread_join(threads[i], NULL);
            close(serverSocket);
            for(int i=0; i<servantCount; ++i)
                freePointer(allServants[i].cities);
            for(int i=clientRequests.currentIndex; i<1024; ++i)
                if(clientRequests.clientSocketFdArray[i]!=-1)
                    close(clientRequests.clientSocketFdArray[i]);
            freePointer(allServants);
            exit(EXIT_FAILURE);
        }

        //Put the request into queue
        returnVal=pthread_mutex_lock(&critMutex);
        if(returnVal!=0)
        {
            perror("Cannot lock mutex in server");
            sigintFlag=1;
            pthread_cond_broadcast(&queueEmpty);
            for(int i=0; i<threadCount; ++i)
                pthread_join(threads[i], NULL);
            for(int i=0; i<servantCount; ++i)
                freePointer(allServants[i].cities);

            for(int i=clientRequests.currentIndex; i<1024; ++i)
                if(clientRequests.clientSocketFdArray[i]!=-1)
                    close(clientRequests.clientSocketFdArray[i]);
            freePointer(allServants);
            close(retrievedSocket);
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        while(clientRequests.activeRequests==1024 && sigintFlag==0)
        {
            pthread_cond_wait(&queueFull, &critMutex);
            if(sigintFlag==1)
            {
                char output[1000];
                arrangeTerminationOutput(output);
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write the termination output of the server");

                pthread_cond_broadcast(&queueEmpty);
                pthread_mutex_unlock(&critMutex);

                for(int i=0; i<threadCount; ++i)
                    pthread_join(threads[i], NULL);

                for(int i=clientRequests.currentIndex; i<1024; ++i)
                    if(clientRequests.clientSocketFdArray[i]!=-1)
                        close(clientRequests.clientSocketFdArray[i]);


                for(int i=0; i<servantCount; ++i)
                    kill(allServants[i].servantPID, SIGINT);

                for(int i=0; i<servantCount; ++i)
                    freePointer(allServants[i].cities);
                freePointer(allServants);
                close(serverSocket);
                return 0;
            }
            if(returnVal!=0)
            {
                perror("Cannot wait on condition in server");
                sigintFlag=1;
                pthread_cond_broadcast(&queueEmpty);
                pthread_mutex_unlock(&critMutex);

                for(int i=0; i<threadCount; ++i)
                    pthread_join(threads[i], NULL);
                for(int i=0; i<servantCount; ++i)
                    freePointer(allServants[i].cities);

                for(int i=clientRequests.currentIndex; i<1024; ++i)
                    if(clientRequests.clientSocketFdArray[i]!=-1)
                        close(clientRequests.clientSocketFdArray[i]);

                freePointer(allServants);
                close(retrievedSocket);
                close(serverSocket);
                exit(EXIT_FAILURE);
            }
        }

        clientRequests.clientSocketFdArray[clientRequests.totalRequests]=retrievedSocket;
        clientRequests.totalRequests++;
        clientRequests.activeRequests++;

        if(clientRequests.totalRequests==1024)
            clientRequests.totalRequests=0;

        pthread_cond_broadcast(&queueEmpty);
        pthread_mutex_unlock(&critMutex);
    }

    //Waiting for the threads
    for(int i=0; i<threadCount; ++i)
        pthread_join(threads[i], NULL);

    close(serverSocket);
    for(int i=0; i<servantCount; ++i)
        freePointer(allServants[i].cities);
    freePointer(allServants);
    return 0;
}

void checkArgc(int count)
{
    if(count!=5)
    {
        char errorMessage[]="You need exactly 5 command line arguments (first being auto inputed) to run the program.\n-p for port number, -t for thread count.\nPlease try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], int* portNo, int* tCount)
{
    int option;
    while((option=getopt(count, arguements, "p:t:"))!=-1)
        switch(option)
        {
            case 'p':
                *portNo=atoi(optarg);
                break;
            case 't':
                *tCount=atoi(optarg);
                break;
            default:
                return -1;
        }

    return 0;
}

void retrieveServantData(char* dataString, struct servantInfo* newServant)
{
    char* word;
    int counter=0;

    newServant->cityCount=0;
    word=strtok(dataString, " ");
    while(word!=NULL)
    {
        if(counter==1)
            newServant->servantPID=atoi(word);
        else if(counter==2)
            newServant->servantPort=atoi(word);
        else if(counter>3)
        {
            struct cityInfo newCity;
            newCity.cityNo=-1;
            strcpy(newCity.cityName, word);
            newServant->cities=addCity(newCity, &(newServant->cities), &(newServant->cityCount));
        }

        counter++;
        word=strtok(NULL, " ");
    }
}

struct servantInfo* addServant(struct servantInfo currentServant, struct servantInfo** allServants, int *servantCount)
{
    if(*servantCount==0)
    {
        *allServants=(struct servantInfo*)malloc(sizeof(struct servantInfo));
        if(allServants==NULL)
            return NULL;
        (*allServants)[0]=currentServant;
        (*servantCount)++;
        return *allServants;
    }

    struct servantInfo* backupArray=(struct servantInfo*)malloc((*servantCount)*sizeof(struct servantInfo));
    if(backupArray==NULL)
        return NULL;

    for(int i=0; i<*servantCount; ++i)
        backupArray[i]=(*allServants)[i];

    *allServants=(struct servantInfo*)realloc(*allServants, (*servantCount)*sizeof(struct servantInfo)+sizeof(struct servantInfo));
    if(*allServants==NULL)
    {
        free(backupArray);
        return(NULL);
    }
    for(int i=0; i<*servantCount; ++i)
        (*allServants)[i]=backupArray[i];
    (*allServants)[*servantCount]=currentServant;

    (*servantCount)++;

    free(backupArray);

    return *allServants;
}

void* threadFunction(void* args)
{
    while(true)
    {
        int returnVal=0;
        pthread_mutex_lock(&critMutex);
        if(sigintFlag==1)
        {
            writeOutput("Server thread exiting due to SIGINT\n", STDOUT_FILENO);
            pthread_cond_broadcast(&queueFull);
            pthread_cond_broadcast(&queueEmpty);
            pthread_mutex_unlock(&critMutex);
            return NULL;
        }
        while(clientRequests.activeRequests==0 && sigintFlag==0)
        {
            if(sigintFlag==1)
            {
                writeOutput("Server thread exiting due to SIGINT\n", STDOUT_FILENO);
                pthread_cond_broadcast(&queueFull);
                pthread_cond_broadcast(&queueEmpty);
                pthread_mutex_unlock(&critMutex);
                return NULL;
            }

            pthread_cond_wait(&queueEmpty, &critMutex);
            if(sigintFlag==1)
            {
                writeOutput("Server thread exiting due to SIGINT\n", STDOUT_FILENO);
                pthread_cond_broadcast(&queueFull);
                pthread_cond_broadcast(&queueEmpty);
                pthread_mutex_unlock(&critMutex);
                return NULL;
            }
            if(returnVal!=0)
            {
                perror("Cannot wait on condition in server thread");
                pthread_mutex_unlock(&critMutex);
                return NULL;
            }
        }
        if(sigintFlag==1)
        {
            writeOutput("Server thread exiting due to SIGINT\n", STDOUT_FILENO);
            pthread_cond_broadcast(&queueFull);
            pthread_cond_broadcast(&queueEmpty);
            pthread_mutex_unlock(&critMutex);
            return NULL;
        }

        int retrievedFd=clientRequests.clientSocketFdArray[clientRequests.currentIndex];
        int retrievedIndex=clientRequests.currentIndex;
        clientRequests.currentIndex++;
        clientRequests.activeRequests--;

        if(clientRequests.currentIndex==1024)
            clientRequests.currentIndex=0;

        pthread_cond_broadcast(&queueFull);
        pthread_mutex_unlock(&critMutex);

        //Retrieving the request
        char* readLine=NULL;
        int lineSize=0;
        struct package readServerInput;
        while(true)
        {
            char readChar[1];
            int bytesread;
            while(((bytesread=read(retrievedFd, readChar, 1))==-1) && (errno==EINTR));
            if(bytesread<0)
            {
                perror("Cannot read from the socket in server");
                freePointer(readLine);
                close(retrievedFd);
                exit(EXIT_FAILURE);
            }
            else if(bytesread==0)
                break;
            else if(readChar[0]!='\n' && readChar[0]!='\r')
            {
                readLine=addCharacterToLine(readChar[0], &readLine, &lineSize);
                if(readLine==NULL)
                {
                    perror("Cannot allocate memory for read socket line in server");
                    freePointer(readLine);
                    close(retrievedFd);
                    exit(EXIT_FAILURE);
                }
            }
            else if(readChar[0]=='\n')
            {
                if(readLine[0]=='c')
                {
                    readServerInput.isClient=1;
                    retrieveRequestData(readLine, &(readServerInput.clientRequest));
                }
                else if(readLine[0]=='s')
                {
                    readServerInput.isClient=0;
                    retrieveServantData(readLine, &(readServerInput.servant));
                }

                free(readLine);
                readLine=NULL;
                break;
            }
        }

        //If the request came from a client
        if(readServerInput.isClient==0)
        {
            allServants=addServant(readServerInput.servant, &allServants, &servantCount);
            if(allServants==NULL)
            {
                perror("Cannot allocate memory for servants in server");
                sigintFlag=1;
                pthread_cond_broadcast(&queueEmpty);
                pthread_cond_broadcast(&queueFull);
                close(retrievedFd);
                exit(EXIT_FAILURE);
            }

            //Servant Loaded Output
            char output[5000];
            arrangeServantLoadedOutput(output, readServerInput.servant);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the server loaded output of the server");

        }
        else
        {
            struct request retrievedRequest=readServerInput.clientRequest;
            //Request Arrived Output
            char output[5000];
            arrangeRequestArrivedOutput(output, retrievedRequest, (*((int*)args)));
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the arrived request output of the server thread");

            int servantPID=-1;
            struct servantInfo relatedServant;
            int result=0;
            int backupResult=0;
            int counter=0;
            if(strcmp(retrievedRequest.city, "NoNe")!=0)
            {
                //Find which servant to connect to
                for(int i=0; i<servantCount; ++i)
                {
                    for(int j=0; j<allServants[i].cityCount; ++j)
                    {
                        if(strcmp(allServants[i].cities[j].cityName, retrievedRequest.city)==0)
                        {
                            servantPID=allServants[i].servantPID;
                            relatedServant.servantPID=allServants[i].servantPID;
                            relatedServant.servantPort=allServants[i].servantPort;
                            break;
                        }
                    }
                    if(servantPID!=-1)
                        break;
                }

                if(servantPID==-1)
                {
                    writeOutput("There is no servant responsible of this city.\n", STDERR_FILENO);
                    result=-1;
                }
                else
                {
                    //Connecting to servant output
                    arrangeServantConnectionOutput(output, servantPID, (*((int*)args)));
                    if(writeOutput(output, STDOUT_FILENO)==-1)
                        perror("Cannot write the connection output of the server thread");

                    int servantSocket;
                    struct sockaddr_in serv_addr;
                    while(((servantSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
                    if(servantSocket<0)
                    {
                        perror("Failed to create socket in server thread");
                        close(retrievedFd);
                        return NULL;
                    }
                    counter=0;

                    memset(&serv_addr, 0, sizeof(serv_addr));
                    serv_addr.sin_family=AF_INET;
                    serv_addr.sin_port=htons(relatedServant.servantPort);
                    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

                    while((returnVal=connect(servantSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
                    if(returnVal<0)
                    {
                        perror("Failed to connect in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        return NULL;
                    }
                    counter=0;

                    //Writing to servant port
                    char* socketInput=arrangeClientSocketInput(&socketInput, retrievedRequest);
                    if(socketInput==NULL)
                    {
                        perror("Cannot allocate memory for socket input in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        return NULL;
                    }

                    while(((returnVal=write(servantSocket, socketInput, strlen(socketInput)))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("Failed to write to the socket in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        free(socketInput);
                        return NULL;
                    }

                    //Reading from servant port
                    while(((returnVal=read(servantSocket, &result, sizeof(int)))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("Failed to read from the socket in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        free(socketInput);
                        return NULL;
                    }
                    close(servantSocket);
                    free(socketInput);
                }
            }
            else
            {
                //Connect all servants
                arrangeServantConnectionOutput(output, servantPID, (*((int*)args)));
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write the connection output of the server thread");

                for(int i=0; i<servantCount; ++i)
                {
                    int servantSocket;
                    struct sockaddr_in serv_addr;
                    while(((servantSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
                    if(servantSocket<0)
                    {
                        perror("Failed to create socket in server thread");
                        close(retrievedFd);
                        return NULL;
                    }
                    counter=0;

                    memset(&serv_addr, 0, sizeof(serv_addr));
                    serv_addr.sin_family=AF_INET;
                    serv_addr.sin_port=htons(allServants[i].servantPort);
                    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

                    while((returnVal=connect(servantSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
                    if(returnVal<0)
                    {
                        perror("Failed to connect in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        return NULL;
                    }

                    //Writing to servant port
                    char* socketInput=arrangeClientSocketInput(&socketInput, retrievedRequest);
                    if(socketInput==NULL)
                    {
                        perror("Cannot allocate memory for socket input in server thread");
                        close(servantSocket);
                        close(retrievedFd);
                        return NULL;
                    }

                    while(((returnVal=write(servantSocket, socketInput, strlen(socketInput)))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("Failed to write to the socket in server (to servant port)");
                        close(servantSocket);
                        close(retrievedFd);
                        free(socketInput);
                        return NULL;
                    }

                    //Reading from servant port
                    while(((returnVal=read(servantSocket, &backupResult, sizeof(int)))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("Failed to read from the socket in server (servant port)");
                        close(servantSocket);
                        close(retrievedFd);
                        free(socketInput);
                        return NULL;
                    }
                    result+=backupResult;
                    close(servantSocket);
                    free(socketInput);
                }
            }

            //Retrieved result output
            arrangeResultRetrieved(output, result, (*((int*)args)));
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the result retrieved output of the server thread");

            while(((returnVal=write(retrievedFd, &result, sizeof(int)))==-1) && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot write to the client socket in server thread");
                close(retrievedFd);
                return NULL;
            }

            handledRequestCount++;
        }

        pthread_mutex_lock(&critMutex);

        close(retrievedFd);
        clientRequests.clientSocketFdArray[retrievedIndex]=-1;

        pthread_mutex_unlock(&critMutex);
    }

    return NULL;
}

void arrangeTerminationOutput(char* output)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[5000];
    char buffer[5000];

    strftime(res, 5000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 5000, ": SIGINT has been received. I handled a total of %d requests. Goodbye.\n", handledRequestCount);
    strcat(output, buffer);
}

void arrangeRequestArrivedOutput(char* output, struct request arrivedRequest, int no)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[5000];
    char buffer[5000];

    strftime(res, 5000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 5000, ": Server-Thread%d: Request arrived \"transactionCount %s, %s, %s, %s\"\n", no, arrivedRequest.type, arrivedRequest.startDate, arrivedRequest.endDate, arrivedRequest.city);
    strcat(output, buffer);
}

void arrangeServantConnectionOutput(char* output, int servantP, int no)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    if(servantP!=-1)
        snprintf(buffer, 3000, ": Server-Thread%d: Contacting servant %d\n", no, servantP);
    else
        snprintf(buffer, 3000, ": Server-Thread%d: Contacting all servants\n", no);

    strcat(output, buffer);
}

void arrangeResultRetrieved(char* output, int result, int no)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Server-Thread%d: Response received: %d, forwarded to client\n", no, result);
    strcat(output, buffer);
}

void arrangeServantLoadedOutput(char* output, struct servantInfo loadedServant)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[5000];
    char buffer[5000];

    strftime(res, 5000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 5000, ": Server: Servant %d present at port %d handling cities %s-%s\n", loadedServant.servantPID, loadedServant.servantPort, loadedServant.cities[0].cityName, loadedServant.cities[(loadedServant.cityCount)-1].cityName);
    strcat(output, buffer);
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

