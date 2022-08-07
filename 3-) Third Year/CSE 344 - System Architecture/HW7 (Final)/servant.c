#include "servant.h"
#include <dirent.h>
#include <sys/mman.h>
#include <semaphore.h>

struct node* dataBinaryTree=NULL;   //an avl tree which holds servant dataset
struct servantInfo currentServant;  //current servants info port/pid/cities
int sigintFlag=0;   //a flag to check if sigint has been received
int handledRequestCount=0;  //a flag to check if sigint has been received

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
    char* dataSetDirectoryPath=NULL;
    int cityNoStart=-1;
    int cityNoEnd=-1;
    char* serverIP=NULL;
    int portNo=-1;
    retrieveArguments(argc, argv, &dataSetDirectoryPath, &cityNoStart, &cityNoEnd, &serverIP, &portNo);
    if(dataSetDirectoryPath==NULL || cityNoEnd==-1 || cityNoEnd==-1 || portNo==-1 || serverIP==NULL)
    {
        char errorMessage[]="Arguments are not provided properly.\nYou need exactly 9 command line arguments (first being auto inputed) to run the program.\n-d for data set directory, -c for city numbers, -r for Server IP, -p for Port number.\nPlease try again.\n\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Retrieving city names
    struct cityInfo* allCities;
    int allCitiesCount=0;
    DIR* dataSetDIR=opendir(dataSetDirectoryPath);
    if(dataSetDIR==NULL)
    {
        perror("Cannot open the dataset directory");
        exit(EXIT_FAILURE);
    }
    struct dirent* dir;
    while((dir=readdir(dataSetDIR)) != NULL)
    {
        if(strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0)
        {
            struct cityInfo newCity;
            newCity.cityNo=-1;
            strcpy(newCity.cityName, dir->d_name);
            allCities=addCity(newCity, &allCities, &allCitiesCount);
            if(allCities==NULL)
            {
                perror("Cannot allocate memory for all cities in servant");
                closedir(dataSetDIR);
                exit(EXIT_FAILURE);
            }
        }
    }
    qsort(allCities, allCitiesCount, sizeof(struct cityInfo), compareCityNames);
    currentServant.cityCount=0;
    closedir(dataSetDIR);

    //Retrieving the pid
    char pathName[100]="/proc/self";
    strcat(pathName, "/stat");
    FILE* file=fopen(pathName, "r");
    if(file==NULL)
    {
        perror("Cannot open proc file in servant");
        free(allCities);
    }
    fscanf(file, "%d", &currentServant.servantPID);
    fclose(file);

    //Initializing the cities of the servant
    for(int i=0; i<allCitiesCount; ++i)
    {
        allCities[i].cityNo=i+1;
        if(i+1>=cityNoStart && i+1<=cityNoEnd)
        {
            currentServant.cities=addCity(allCities[i], &(currentServant.cities), &(currentServant.cityCount));
            if(currentServant.cities==NULL)
            {
                perror("Cannot allocate memory for cities of the servant");
                freePointer(allCities);
                exit(EXIT_FAILURE);
            }
        }
    }

    freePointer(allCities);

    //Creating a semaphore for shared memory
    sem_t* critSem;
    int returnVal;
    while(((critSem=sem_open("servantSem", O_CREAT, 0777, 1))==NULL) && errno==EINTR);
    if(critSem==SEM_FAILED)
    {
        perror("Cannot open the semaphore for critical region in servant");
        free(currentServant.cities);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_wait(critSem)))==-1 && errno==EINTR);

    //Opening a shared memory for PORT number determination
    int portFd;
    int* portAdd;
    int sharedMemoryExists=0;
    while(((portFd=shm_open("/PORTNumbers", O_EXCL | O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(portFd==-1 && errno!=EEXIST)
    {
        perror("Cannot create shared memory for Port Numbers in servant");
        while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);
        sem_close(critSem);
        sem_unlink("servantSem");
        free(currentServant.cities);
        exit(EXIT_FAILURE);
    }
    else if(errno==EEXIST)
    {
        sharedMemoryExists=1;
        while(((portFd=shm_open("/PORTNumbers", O_RDWR, 0777))==-1) && errno==EINTR);
        if(portFd==-1)
        {
            perror("Cannot create shared memory for Port Numbers in servant");
            while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);
            sem_close(critSem);
            sem_unlink("servantSem");
            free(currentServant.cities);
            exit(EXIT_FAILURE);
        }

        while(((portAdd=mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, portFd, 0))==MAP_FAILED) && errno==EINTR);
        if(portAdd==MAP_FAILED)
        {
            perror("Mapping for port numbers failed in servant");
            while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);
            close(portFd);
            shm_unlink("/PORTNumbers");
            sem_close(critSem);
            sem_unlink("servantSem");
            free(currentServant.cities);
            exit(EXIT_FAILURE);
        }

        close(portFd);
    }

    if(sharedMemoryExists==0)
    {
        while(((returnVal=ftruncate(portFd, sizeof(int)))==-1) && errno==EINTR);
        if(returnVal==-1)
        {
            perror("Allocating memory for port numbers in shared memory failed in servant");
            while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);
            close(portFd);
            shm_unlink("/PORTNumbers");
            sem_close(critSem);
            sem_unlink("servantSem");
            free(currentServant.cities);
            exit(EXIT_FAILURE);
        }

        while(((portAdd=mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, portFd, 0))==MAP_FAILED) && errno==EINTR);
        if(portAdd==MAP_FAILED)
        {
            perror("Mapping for port numbers failed in servant");
            while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);
            close(portFd);
            shm_unlink("/PORTNumbers");
            sem_close(critSem);
            sem_unlink("servantSem");
            free(currentServant.cities);
            exit(EXIT_FAILURE);
        }

        close(portFd);

        //Initializing the port numbers
        int startInt=portNo;
        memcpy(portAdd, &startInt, sizeof(int));   //Used for ended flag
    }

    (*portAdd)+=17;
    currentServant.servantPort=(*portAdd);
    while(((returnVal=sem_post(critSem)))==-1 && errno==EINTR);

    //Saving city data
    //Data Loaded Output
    char output[3000];
    arrangeDataLoadedOutput(output, currentServant.servantPID, currentServant.cities[0].cityName, currentServant.cities[(currentServant.cityCount)-1].cityName);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the data loaded output of servant");

    //Creating the binary tree
    struct transactions* allCityTrans=NULL;

    for(int i=0; i<currentServant.cityCount; ++i)
    {
        int allCityTransCount=0;
        char path[3000];
        strcpy(path, dataSetDirectoryPath);
        strcat(path, "/");
        strcat(path, currentServant.cities[i].cityName);

        DIR* cityDir=opendir(path);
        if(cityDir==NULL)
        {
            writeOutput("The directory with the given city name cannot be opened.\n", STDERR_FILENO);
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            free(currentServant.cities);
            exit(EXIT_FAILURE);
        }

        while((dir=readdir(cityDir))!=NULL)
        {
            if(strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0)
            {
                char tranDate[500];
                strcpy(tranDate, dir->d_name);

                strcpy(path, dataSetDirectoryPath);
                strcat(path, "/");
                strcat(path, currentServant.cities[i].cityName);
                strcat(path, "/");
                strcat(path, dir->d_name);

                int dateFile=openFile(path, O_RDONLY, 0555);
                if(dateFile==-1)
                {
                    writeOutput("Cannot open date file.\n", STDERR_FILENO);
                    sem_close(critSem);
                    closedir(cityDir);
                    sem_unlink("servantSem");
                    shm_unlink("/PORTNumbers");
                    munmap("/PORTNumbers", sizeof(int));
                    free(currentServant.cities);
                    exit(EXIT_FAILURE);
                }

                char* tranType=NULL;
                int typeNameSize=0;
                int typeRetrieved=0;
                int idRetrieved=0;
                while(true)
                {
                    char readChar[1];
                    int bytesread;
                    while(((bytesread=read(dateFile, readChar, 1))==-1) && (errno==EINTR));
                    if(bytesread==-1)
                    {
                        perror("Cannot read from the date file");
                        sem_close(critSem);
                        close(dateFile);
                        closedir(cityDir);
                        sem_unlink("servantSem");
                        shm_unlink("/PORTNumbers");
                        munmap("/PORTNumbers", sizeof(int));
                        free(currentServant.cities);
                        freePointer(tranType);
                        freePointer(allCityTrans);
                        exit(EXIT_FAILURE);
                    }
                    else if(bytesread==0)
                        break;
                    else if(readChar[0]!='\n' && readChar[0]!='\r' && readChar[0]!=' ' && readChar[0]!='\t' && typeRetrieved==0 && idRetrieved==1)
                    {
                        tranType=addCharacterToLine(readChar[0], &tranType, &typeNameSize);
                        if(tranType==NULL)
                        {
                            perror("Cannot allocate memory for transaction type name in servant");
                            sem_close(critSem);
                            close(dateFile);
                            closedir(cityDir);
                            sem_unlink("servantSem");
                            shm_unlink("/PORTNumbers");
                            munmap("/PORTNumbers", sizeof(int));
                            free(currentServant.cities);
                            freePointer(tranType);
                            freePointer(allCityTrans);
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if(readChar[0]=='\n')
                    {
                        typeRetrieved=0;
                        idRetrieved=0;
                        typeNameSize=0;
                    }
                    else if(readChar[0]==' ' || readChar[0]=='\t')
                    {
                        if(idRetrieved==0)
                            idRetrieved=1;
                        else if(typeNameSize!=0 && idRetrieved==1 && typeRetrieved==0)
                            typeRetrieved=1;
                        else if(typeRetrieved==1 && tranType!=NULL)
                        {
                            struct transactions newTrans;
                            strcpy(newTrans.date, dir->d_name);
                            strcpy(newTrans.type, tranType);
                            allCityTrans=addTransaction(newTrans, &(allCityTrans), &allCityTransCount);
                            freePointer(tranType);
                            tranType=NULL;
                        }
                    }
                }
                close(dateFile);
            }
        }

        //Adding to the binary tree
        dataBinaryTree=insert(dataBinaryTree, currentServant.cities[i].cityName, allCityTransCount, allCityTrans);
        closedir(cityDir);
    }

    if(sigintFlag==1)
    {
        arrangeTerminationOutput(output, currentServant.servantPID);
        if(writeOutput(output, STDOUT_FILENO)==-1)
            perror("Cannot write the termination output of servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);
        return 0;
    }

    //Connecting with server to give servant info
    int serverSocket;
    struct sockaddr_in serv_addr;
    int counter=0;
    while(((serverSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
    if(serverSocket<0)
    {
        perror("Failed to create socket in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);
    }
    counter=0;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portNo);
    serv_addr.sin_addr.s_addr=inet_addr(serverIP);

    while((returnVal=connect(serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Failed to connect in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        close(serverSocket);
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);
    }
    counter=0;

    //Writing to socket
    char* socketInput=arrangeSocketInput(&socketInput, currentServant);
    if(socketInput==NULL)
    {
        perror("Cannot allocate memory for socketInput of server in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        close(serverSocket);
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);

    }
    while(((returnVal=write(serverSocket, socketInput, strlen(socketInput)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Failed to write to the socket in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        close(serverSocket);
        free(socketInput);
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);
    }

    close(serverSocket);
    free(socketInput);

    //Servant Operations
    //Operation Start Output
    arrangeServantStartOutput(output, currentServant.servantPID, currentServant.servantPort);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the operation start output of servant");

    if(sigintFlag==1)
    {
        arrangeTerminationOutput(output, currentServant.servantPID);
        if(writeOutput(output, STDOUT_FILENO)==-1)
            perror("Cannot write the termination output of servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        delete_tree(dataBinaryTree);
        free(currentServant.cities);
        return 0;
    }

    int servantSocket;
    while(((servantSocket=socket(AF_INET, SOCK_STREAM, 0))<0) && (errno==EINTR || counter<10)){counter++;}
    if(servantSocket<0)
    {
        perror("Failed to create socket in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        exit(EXIT_FAILURE);
    }
    counter=0;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(currentServant.servantPort);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    while((returnVal=bind(servantSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Bind failed in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        close(servantSocket);
        exit(EXIT_FAILURE);
    }
    counter=0;

    while((returnVal=listen(servantSocket, 1024))<0 && (errno==EINTR || counter<10)){counter++;}
    if(returnVal<0)
    {
        perror("Failed to listen in servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        free(currentServant.cities);
        delete_tree(dataBinaryTree);
        close(servantSocket);
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        arrangeTerminationOutput(output, currentServant.servantPID);
        if(writeOutput(output, STDOUT_FILENO)==-1)
            perror("Cannot write the termination output of servant");
        sem_close(critSem);
        sem_unlink("servantSem");
        shm_unlink("/PORTNumbers");
        munmap("/PORTNumbers", sizeof(int));
        delete_tree(dataBinaryTree);
        close(servantSocket);
        free(currentServant.cities);
        return 0;
    }

    //Waiting for requests
    atexit(exitFunction);
    while(true)
    {
        int* retrievedServantSocket=(int*)malloc(sizeof(int));
        if(retrievedServantSocket==NULL)
        {
            perror("Cannot allocate memory for servant socket in servant");
            sigintFlag=1;
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            close(servantSocket);
            pthread_exit(NULL);
        }
        int addrlen=sizeof(serv_addr);
        while(((*retrievedServantSocket)=accept(servantSocket, (struct sockaddr*)&serv_addr, (socklen_t*)&addrlen))<0 && errno==EINTR && sigintFlag==0);
        if(sigintFlag==1)
        {
            arrangeTerminationOutput(output, currentServant.servantPID);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the termination output of servant");

            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            freePointer(retrievedServantSocket);
            close(servantSocket);
            pthread_exit(NULL);
        }

        if(retrievedServantSocket<0)
        {
            perror("Failed to accept in servant");
            sigintFlag=1;
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            freePointer(retrievedServantSocket);
            close(servantSocket);
            pthread_exit(NULL);
        }
        handledRequestCount++;

        //Creating a thread to handle the request
        pthread_t thread;
        pthread_attr_t threadAttr;
        int returnVal=pthread_attr_init(&threadAttr);
        if(returnVal!=0)
        {
            char errorMessage[]="Failed to initialize pthread attributes\n";
            while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
            sigintFlag=1;
            close(*retrievedServantSocket);
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            freePointer(retrievedServantSocket);
            close(servantSocket);
            pthread_exit(NULL);
        }

        returnVal=pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
        if(returnVal!=0)
        {
            char errorMessage[]="Failed to set detached state.\n";
            while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
            sigintFlag=1;
            pthread_attr_destroy(&threadAttr);
            close(*retrievedServantSocket);
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            freePointer(retrievedServantSocket);
            close(servantSocket);
            pthread_exit(NULL);
        }

        returnVal=pthread_create(&thread, &threadAttr, threadFunction, retrievedServantSocket);
        if(returnVal!=0)
        {
            char errorMessage[]="Failed to create the servant thread.\n";
            while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
            while((pthread_attr_destroy(&threadAttr))==-1 && errno==EINTR);
            sigintFlag=1;
            close(*retrievedServantSocket);
            sem_close(critSem);
            sem_unlink("servantSem");
            shm_unlink("/PORTNumbers");
            munmap("/PORTNumbers", sizeof(int));
            freePointer(retrievedServantSocket);
            close(servantSocket);
            pthread_exit(NULL);
        }

        pthread_attr_destroy(&threadAttr);
    }
    close(servantSocket);
    sem_close(critSem);
    sem_unlink("servantSem");
    shm_unlink("/PORTNumbers");
    munmap("/PORTNumbers", sizeof(int));

    return 0;
}

void checkArgc(int count)
{
    if(count!=9)
    {
        char errorMessage[]="You need exactly 9 command line arguments (first being auto inputed) to run the program.\n-d for data set directory, -c for city numbers, -r for Server IP, -p for Port number.\nPlease try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], char** path, int* start, int* end, char** ip, int* portNo)
{
    int option;
    while((option=getopt(count, arguements, "d:c:r:p:"))!=-1)
        switch(option)
        {
            case 'd':
                *path=optarg;
                break;
            case 'c':
                retrieveCityNumbers(optarg, start, end);
                break;
            case 'r':
                *ip=optarg;
                break;
            case 'p':
                *portNo=atoi(optarg);
                break;
            default:
                return -1;
        }

    return 0;
}

void retrieveCityNumbers(char* numbersString, int* start, int* end)
{
    char* word;
    int counter=0;

    word=strtok(numbersString, "-");
    while(word!=NULL)
    {
        if(counter==0)
            *start=atoi(word);
        else if(counter==1)
        {
            *end=atoi(word);
            break;
        }

        counter++;
        word=strtok(NULL, " ");
    }
}

int compareCityNames(const void * a, const void * b)
{
    char* string1=((struct cityInfo*)a)->cityName;
    char* string2=((struct cityInfo*)b)->cityName;
    return strcmp(string1, string2);
}

char* arrangeSocketInput(char** socketInput, struct servantInfo currentservant)
{
    char pidBuffer[500];
    char portBuffer[500];
    char countBuffer[500];
    sprintf(pidBuffer, "%d", currentservant.servantPID);
    sprintf(portBuffer, "%d", currentservant.servantPort);
    sprintf(countBuffer, "%d", currentservant.cityCount);

    int allocationSize=2+2+strlen(pidBuffer)+strlen(portBuffer)+strlen(countBuffer)+3;
    for(int i=0; i<currentservant.cityCount; ++i)
        allocationSize+=strlen(currentservant.cities[i].cityName)+1;
    allocationSize--;

    *socketInput=(char*)malloc(allocationSize);
    if(*socketInput==NULL)
        return NULL;

    strcpy(*socketInput, "s ");
    strcat(*socketInput, pidBuffer);
    strcat(*socketInput, " ");
    strcat(*socketInput, portBuffer);
    strcat(*socketInput, " ");
    strcat(*socketInput, countBuffer);
    strcat(*socketInput, " ");
    for(int i=0; i<currentservant.cityCount; ++i)
    {
        strcat(*socketInput, currentservant.cities[i].cityName);
        if(i!=currentservant.cityCount-1)
            strcat(*socketInput, " ");
    }
    strcat(*socketInput, "\n");

    (*socketInput)[allocationSize-1]='\0';

    return (*socketInput);
}

void arrangeDataLoadedOutput(char* output, int servantPid, char* firstCityName, char* secondCityName)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Servant %d: loaded dataset, cities %s-%s\n", servantPid, firstCityName, secondCityName);
    strcat(output, buffer);
}

void arrangeServantStartOutput(char* output, int pid, int port)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Servant %d: listening at port %d\n", pid, port);
    strcat(output, buffer);
}

void arrangeTerminationOutput(char* output, int pid)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[3000];
    char buffer[3000];

    strftime(res, 3000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 3000, ": Servant %d: termination message received, handled %d requests in total.\n", pid, handledRequestCount);
    strcat(output, buffer);
}

void* threadFunction(void* args)
{
    //Retrieving arguments
    int fd=*((int*)args);

    char* readLine=NULL;
    int lineSize=0;
    struct package readServerInput;
    while(true)
    {
        char readChar[1];
        int bytesread;
        while(((bytesread=read(fd, readChar, 1))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the socket in servant");
            close(fd);
            freePointer(readLine);
            freePointer(args);
            args=NULL;
            exit(EXIT_FAILURE);
        }
        else if(bytesread==0)
            break;
        else if(readChar[0]!='\n' && readChar[0]!='\r')
        {
            readLine=addCharacterToLine(readChar[0], &readLine, &lineSize);
            if(readLine==NULL)
            {
                perror("Cannot allocate memory for read socket line in servant");
                freePointer(readLine);
                close(fd);
                freePointer(args);
                args=NULL;
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

            freePointer(readLine);
            readLine=NULL;
            break;
        }
    }

    //Finding out the answer
    int response=0;
    if(strcmp(readServerInput.clientRequest.city, "NoNe")!=0)
    {
        struct node* cityNode=search(&dataBinaryTree, readServerInput.clientRequest.city);
        for(int i=0; i<cityNode->transactionCount; ++i)
        {
            int dateResult=compareDates(readServerInput.clientRequest.endDate, readServerInput.clientRequest.startDate, cityNode->cityTransactions[i].date);
            if(dateResult==1 && strcmp(cityNode->cityTransactions[i].type, readServerInput.clientRequest.type)==0)
                response++;
        }
    }
    else
    {
        for(int j=0; j<currentServant.cityCount; ++j)
        {
            struct node* cityNode=search(&dataBinaryTree, currentServant.cities[j].cityName);
            for(int i=0; i<cityNode->transactionCount; ++i)
            {
                int dateResult=compareDates(readServerInput.clientRequest.endDate, readServerInput.clientRequest.startDate, cityNode->cityTransactions[i].date);
                if(dateResult==1 && strcmp(cityNode->cityTransactions[i].type, readServerInput.clientRequest.type)==0)
                    response++;
            }
        }
    }

    //Writing the answer
    int returnVal=0;
    while(((returnVal=write(fd, &response, sizeof(int)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot write to the server the response");
        close(fd);
        freePointer(args);
        args=NULL;
        return NULL;
    }

    close(fd);
    freePointer(args);
    args=NULL;

    return NULL;
}

struct transactions* addTransaction(struct transactions currentTransaction, struct transactions** allTransactions, int *transactionCount)
{
    if(*transactionCount==0)
    {
        *allTransactions=(struct transactions*)malloc(sizeof(struct transactions));
        if(allTransactions==NULL)
            return NULL;
        (*allTransactions)[0]=currentTransaction;
        (*transactionCount)++;
        return *allTransactions;
    }

    /*struct transactions* backupArray=(struct transactions*)malloc((*transactionCount)*sizeof(struct transactions));
    if(backupArray==NULL)
        return NULL;*/

    /*for(int i=0; i<*transactionCount; ++i)
        backupArray[i]=(*allTransactions)[i];*/

    *allTransactions=(struct transactions*)realloc(*allTransactions, (*transactionCount)*sizeof(struct transactions)+sizeof(struct transactions));
    if(*allTransactions==NULL)
    {
        //free(backupArray);
        return(NULL);
    }
    /*for(int i=0; i<*transactionCount; ++i)
        (*allTransactions)[i]=backupArray[i];*/
    (*allTransactions)[*transactionCount]=currentTransaction;

    (*transactionCount)++;

    //free(backupArray);

    return *allTransactions;
}

int compareDates(char* endD, char* startT, char* dateE)
{
    char end[500];
    char start[500];
    char date[500];
    char* endB;
    char* startB;
    char* dateB;
    strcpy(end, endD);
    strcpy(start, startT);
    strcpy(date, dateE);
    int endDay, endMonth, endYear;
    int startDay, startMonth, startYear;
    int dateDay, dateMonth, dateYear;
    endB=end;
    endDay=strtol(endB,&endB,10);
    endMonth=strtol(endB+1,&endB,10);
    endYear=strtol(endB+1,&endB,10);
    startB=start;
    startDay=strtol(startB,&startB,10);
    startMonth=strtol(startB+1,&startB,10);
    startYear=strtol(startB+1,&startB,10);
    dateB=date;
    dateDay=strtol(dateB,&dateB,10);
    dateMonth=strtol(dateB+1,&dateB,10);
    dateYear=strtol(dateB+1,&dateB,10);

    //Start-Date Comparison => Value bigger than 0 expected
    int isBigger;
    isBigger=calculateDateDifference(dateDay, dateMonth, dateYear, startDay, startMonth, startYear);

    //End-Date Comparison => Value smaller than 0 expected
    int isSmaller;
    isSmaller=calculateDateDifference(endDay, endMonth, endYear, dateDay, dateMonth, dateYear);

    if(isBigger>=0 && isSmaller>=0)
        return 1;
    else
        return 0;

}

int calculateDateDifference(int d1, int m1, int y1, int d2, int m2, int y2)
{
    if(d1-d2<0)
    {
        d1+=30;
        m1--;
    }
    if(m1-m2<0)
    {
        m1+=12;
        y1--;
    }
    if(y1-y2<0)
        return -1;
    else
        return 1;
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

void exitFunction(void)
{
    freePointer(currentServant.cities);
    delete_tree(dataBinaryTree);
}
