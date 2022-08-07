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
#include <stdbool.h>
#include <semaphore.h>
#include <sys/mman.h>

struct chefInfo{
    char neededIng1;
    char neededIng2;
    int no;
};

struct forbiddenPairs
{
    char pair[6][2];
};

struct semSharedMem
{
    sem_t critSem;
    sem_t emptySem;
    sem_t fullSem;

    sem_t chefSem[6];
};

void checkArgc(int count);
int retrieveArguments(int count, char* arguements[], char** dataPath);
int openFile(char* filePath, int flags, mode_t mode);
void arrangeWholesalerOutput1(char* output, int id, char ingLetter1, char ingLetter2);
int writeOutput(char* output, int fileFd);
void arrangeChefOutput(char* output, int no, int id, char ingLetter1, char ingLetter2, char array1, char array2);
void arrangeChefOutput2(char* output, int no, int id, char ingLetter, char ingLetterOther, int count);
void closeAll();

int endedFd;
int* endedAdd;
int arrayFd;
char* arrayAdd;
int semShmFd;
struct semSharedMem* semAdd;
int forbiddenFd;
struct forbiddenPairs* forbiddenAdd;

int main(int argc, char* argv[])
{
    //Checking the command line arguments
    checkArgc(argc);

    //Retrieving the command line inputs
    char* dataFilePath=NULL;
    if(retrieveArguments(argc, argv, &dataFilePath)==-1)
    {
        char errorMessage[]="There is an invalid command line argument. You can only use \"-i\". Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Checking argument validity
    if(dataFilePath==NULL)
    {
        char errorMessage[]="Arguments are not provided properly. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Opening shared memory for ended flag
    int returnVal;
    while(((endedFd=shm_open("/EndedFlag", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(endedFd==-1)
    {
        perror("Cannot create shared memory for ended flag");
        exit(EXIT_FAILURE);
    }

    while(((returnVal=ftruncate(endedFd, sizeof(int)*2))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Allocating memory for ended flag in shared memory failed");
        close(endedFd);
        shm_unlink("/EndedFlag");
        exit(EXIT_FAILURE);
    }

    while(((endedAdd=mmap(NULL, sizeof(int)*2, PROT_READ | PROT_WRITE, MAP_SHARED, endedFd, 0))==MAP_FAILED) && errno==EINTR);
    if(endedAdd==MAP_FAILED)
    {
        perror("Mapping for ended flag failed");
        close(endedFd);
        shm_unlink("/EndedFlag");
        exit(EXIT_FAILURE);
    }

    close(endedFd);

    //Initializing the ended flag
    int startInt=0;
    memcpy(endedAdd, &startInt, sizeof(int));   //Used for ended flag
    memcpy(endedAdd+sizeof(int), &startInt, sizeof(int));   //Used for total desserts

    //Opening shared memory for the array
    while(((arrayFd=shm_open("/ArrayMem", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(arrayFd==-1)
    {
        perror("Cannot create shared memory for array");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=ftruncate(arrayFd, sizeof(char)*2))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Allocating memory for array in shared memory failed");
        close(arrayFd);
        shm_unlink("/ArrayMem");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    while(((arrayAdd=mmap(NULL, sizeof(char)*2, PROT_READ | PROT_WRITE, MAP_SHARED, arrayFd, 0))==MAP_FAILED) && errno==EINTR);
    if(arrayAdd==MAP_FAILED)
    {
        perror("Mapping for array failed");
        close(arrayFd);
        shm_unlink("/ArrayMem");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    close(arrayFd);

    //Initializing the array
    char startChar='X';
    memcpy(arrayAdd, &startChar, sizeof(char));
    memcpy(arrayAdd+sizeof(char), &startChar, sizeof(char));

    //Creating shared memory for unnamed semaphores
    while(((semShmFd=shm_open("/SemMem", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(semShmFd==-1)
    {
        perror("Cannot create shared memory for semaphores");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=ftruncate(semShmFd, sizeof(struct semSharedMem)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Allocating memory for array in shared memory failed");
        close(semShmFd);
        shm_unlink("/SemMem");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        exit(EXIT_FAILURE);
    }

    while(((semAdd=mmap(NULL, sizeof(struct semSharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, semShmFd, 0))==MAP_FAILED) && errno==EINTR);
    if(semAdd==MAP_FAILED)
    {
        perror("Mapping for semaphores failed");
        close(semShmFd);
        shm_unlink("/SemMem");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        exit(EXIT_FAILURE);
    }

    close(semShmFd);

    //Opening and Initializing the semaphores
    while(((returnVal=sem_init(&((*semAdd).critSem), 1, 1))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for critical region");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).emptySem), 1, 1))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for empty");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).fullSem), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[0]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[1]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[2]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[3]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[4]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        sem_close(&((*semAdd).chefSem[3]));
        sem_destroy(&((*semAdd).chefSem[3]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=sem_init(&((*semAdd).chefSem[5]), 1, 0))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot open the semaphore for full");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        sem_close(&((*semAdd).chefSem[3]));
        sem_destroy(&((*semAdd).chefSem[3]));
        sem_close(&((*semAdd).chefSem[4]));
        sem_destroy(&((*semAdd).chefSem[4]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    //Opening shared memory for forbiddenIngredients
    while(((forbiddenFd=shm_open("/ForbiddenMem", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(forbiddenFd==-1)
    {
        perror("Cannot create shared memory for forbidden ingredients");
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        sem_close(&((*semAdd).chefSem[3]));
        sem_destroy(&((*semAdd).chefSem[3]));
        sem_close(&((*semAdd).chefSem[4]));
        sem_destroy(&((*semAdd).chefSem[4]));
        sem_close(&((*semAdd).chefSem[5]));
        sem_destroy(&((*semAdd).chefSem[5]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((returnVal=ftruncate(forbiddenFd, sizeof(struct forbiddenPairs)))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Allocating memory for forbidden ingredients in shared memory failed");
        close(forbiddenFd);
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        shm_unlink("/ForbiddenMem");
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        sem_close(&((*semAdd).chefSem[3]));
        sem_destroy(&((*semAdd).chefSem[3]));
        sem_close(&((*semAdd).chefSem[4]));
        sem_destroy(&((*semAdd).chefSem[4]));
        sem_close(&((*semAdd).chefSem[5]));
        sem_destroy(&((*semAdd).chefSem[5]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    while(((forbiddenAdd=mmap(NULL, sizeof(struct forbiddenPairs), PROT_READ | PROT_WRITE, MAP_SHARED, forbiddenFd, 0))==MAP_FAILED) && errno==EINTR);
    if(forbiddenAdd==MAP_FAILED)
    {
        perror("Mapping for forbidden ingredients failed");
        close(forbiddenFd);
        shm_unlink("/EndedFlag");
        munmap("/EndedFlag", sizeof(int)*2);
        shm_unlink("/ArrayMem");
        munmap("/ArrayMem", sizeof(char)*2);
        shm_unlink("/ForbiddenMem");
        sem_close(&((*semAdd).critSem));
        sem_destroy(&((*semAdd).critSem));
        sem_close(&((*semAdd).emptySem));
        sem_destroy(&((*semAdd).emptySem));
        sem_close(&((*semAdd).fullSem));
        sem_destroy(&((*semAdd).fullSem));
        sem_close(&((*semAdd).chefSem[0]));
        sem_destroy(&((*semAdd).chefSem[0]));
        sem_close(&((*semAdd).chefSem[1]));
        sem_destroy(&((*semAdd).chefSem[1]));
        sem_close(&((*semAdd).chefSem[2]));
        sem_destroy(&((*semAdd).chefSem[2]));
        sem_close(&((*semAdd).chefSem[3]));
        sem_destroy(&((*semAdd).chefSem[3]));
        sem_close(&((*semAdd).chefSem[4]));
        sem_destroy(&((*semAdd).chefSem[4]));
        sem_close(&((*semAdd).chefSem[5]));
        sem_destroy(&((*semAdd).chefSem[5]));
        shm_unlink("/SemMem");
        munmap("/SemMem", sizeof(struct semSharedMem));
        exit(EXIT_FAILURE);
    }

    close(forbiddenFd);

    //Initializing forbidden ingredients
    for(int i=0; i<6; ++i)
        for(int j=0; j<2; j++)
            (*forbiddenAdd).pair[i][j]=startChar;

    //Creating the chefs
    struct chefInfo chefs[6];
    int currentChefCount=0;

    for(int i=0; i<6; i++)
    {
        int chefID=fork();
        if(chefID==-1)
        {
            perror("Fork failed");

            //Exit all chefs
            *endedAdd=1;
            for(int i=0; i<currentChefCount; ++i)
               sem_post(&((*semAdd).chefSem[i]));
            for(int i=0; i<currentChefCount; ++i)
                waitpid(-1, NULL, 0);

            closeAll();
            exit(EXIT_FAILURE);
        }
        else if(chefID>0)
        {
            chefs[i].no=i;
            currentChefCount++;
        }
        else if(chefID==0)
        {
            //Chef processing here
            if(i==0)
            {
                chefs[i].neededIng1='W';
                chefs[i].neededIng2='S';
            }
            else if(i==1)
            {
                chefs[i].neededIng1='F';
                chefs[i].neededIng2='W';
            }
            else if(i==2)
            {
                chefs[i].neededIng1='S';
                chefs[i].neededIng2='F';
            }
            else if(i==3)
            {
                chefs[i].neededIng1='M';
                chefs[i].neededIng2='F';
            }
            else if(i==4)
            {
                chefs[i].neededIng1='M';
                chefs[i].neededIng2='W';
            }
            else
            {
                chefs[i].neededIng1='S';
                chefs[i].neededIng2='M';
            }

            while(((returnVal=sem_wait(&((*semAdd).critSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot wait on critSem in chef");

                (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                closeAll();
                return 0;
            }

            char output[200];
            arrangeChefOutput(output, i, getpid(), chefs[i].neededIng1, chefs[i].neededIng2, *(arrayAdd), *(arrayAdd+sizeof(char)));
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write to stdout in chef");

            while(((returnVal=sem_post(&((*semAdd).critSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot post on critSem in chef");

                sem_post(&((*semAdd).critSem));
                (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                closeAll();
                return 0;
            }

            int dessertsMade=0;
            while(true)
            {
                while(((returnVal=sem_wait(&((*semAdd).chefSem[i]))))==-1 && errno==EINTR);
                if(returnVal==-1)
                {
                    perror("Cannot wait on chefSem in chef");

                    (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                    (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                    int val;
                    int val2;
                    sem_getvalue(&((*semAdd).emptySem), &val);
                    sem_getvalue(&((*semAdd).chefSem[i]), &val2);
                    if(val==0 && val2==1)
                        sem_post(&((*semAdd).emptySem));

                    closeAll();
                    return dessertsMade;
                }
                if(*endedAdd==1)
                {
                    snprintf(output, 200, "chef%d (pid %d) is exiting\n", i, getpid());
                    if(writeOutput(output, STDOUT_FILENO)==-1)
                        perror("Cannot write to stdout in chef");

                    closeAll();
                    return dessertsMade;
                }

                while(((returnVal=sem_wait(&((*semAdd).critSem))))==-1 && errno==EINTR);
                if(returnVal==-1)
                {
                    perror("Cannot wait on critSem in chef");

                    (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                    (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                    sem_post(&((*semAdd).emptySem));
                    closeAll();
                    return dessertsMade;
                }

                //Consuming the ingredients
                arrangeChefOutput2(output, i, getpid(), *(arrayAdd), *(arrayAdd+sizeof(char)), 1);
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write to stdout in chef");

                arrangeChefOutput2(output, i, getpid(), *(arrayAdd+sizeof(char)), *(arrayAdd), 2);
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write to stdout in chef");

                snprintf(output, 200, "chef%d (pid %d) is preparing the dessert, (%c%c)\n", i, getpid(), *(arrayAdd), *(arrayAdd+sizeof(char)));
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write to stdout in chef");

                //Delivering the dessert
                (*(endedAdd+sizeof(int)))++;
                dessertsMade++;

                snprintf(output, 200, "chef%d (pid %d) has delivered the dessert, (%c%c)\n", i, getpid(), *(arrayAdd), *(arrayAdd+sizeof(char)));
                if(writeOutput(output, STDOUT_FILENO)==-1)
                    perror("Cannot write to stdout in chef");

                //Emptying the array
                *(arrayAdd)='X';
                *(arrayAdd+sizeof(char))='X';

                while(((returnVal=sem_post(&((*semAdd).critSem))))==-1 && errno==EINTR);
                if(returnVal==-1)
                {
                    perror("Cannot post on critSem in chef");

                    (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                    (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                    sem_post(&((*semAdd).critSem));
                    sem_post(&((*semAdd).emptySem));
                    closeAll();
                    return dessertsMade;
                }

                while(((returnVal=sem_post(&((*semAdd).emptySem))))==-1 && errno==EINTR);
                if(returnVal==-1)
                {
                    perror("Cannot post on emptySem in chef");

                    (*forbiddenAdd).pair[i][0]=chefs[i].neededIng1;
                    (*forbiddenAdd).pair[i][1]=chefs[i].neededIng2;

                    sem_post(&((*semAdd).emptySem));
                    closeAll();
                    return dessertsMade;
                }
            }

            closeAll();
            return dessertsMade;
        }
    }

    //Opening the pusher process
    int pusherId=fork();
    if(pusherId==-1)
    {
        perror("For failed to create the pusher");

        //Exit all chefs
        *endedAdd=1;
        for(int i=0; i<currentChefCount; ++i)
               sem_post(&((*semAdd).chefSem[i]));
        for(int i=0; i<6; ++i)
            waitpid(-1, NULL, 0);

        closeAll();
        exit(EXIT_FAILURE);
    }
    else if(pusherId==0)
    {
        for(int i=0; i<6; i++)
        {
            if(i==0)
            {
                chefs[i].neededIng1='W';
                chefs[i].neededIng2='S';
            }
            else if(i==1)
            {
                chefs[i].neededIng1='F';
                chefs[i].neededIng2='W';
            }
            else if(i==2)
            {
                chefs[i].neededIng1='S';
                chefs[i].neededIng2='F';
            }
            else if(i==3)
            {
                chefs[i].neededIng1='M';
                chefs[i].neededIng2='F';
            }
            else if(i==4)
            {
                chefs[i].neededIng1='M';
                chefs[i].neededIng2='W';
            }
            else
            {
                chefs[i].neededIng1='S';
                chefs[i].neededIng2='M';
            }
        }

        while(true)
        {
            //Pusher processing here
            while(((returnVal=sem_wait(&((*semAdd).fullSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot wait on fullSem in pusher");
                (*endedAdd)=1;
                sem_post(&((*semAdd).emptySem));
            }

            if(*endedAdd==1)
            {
                for(int i=0; i<6; ++i)
                {
                    while(((returnVal=sem_post(&((*semAdd).chefSem[i]))))==-1 && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("sem post failed");
                        sem_post(&((*semAdd).chefSem[i]));
                    }
                }
                closeAll();
                return 0;
            }

            while(((returnVal=sem_wait(&((*semAdd).critSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot wait on critSem in pusher");

                (*endedAdd)=1;
                sem_post(&((*semAdd).emptySem));
                for(int i=0; i<6; ++i)
                {
                    while(((returnVal=sem_post(&((*semAdd).chefSem[i]))))==-1 && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("sem post failed");
                        sem_post(&((*semAdd).chefSem[i]));
                    }
                }
                closeAll();
                exit(EXIT_FAILURE);
            }

            for(int i=0; i<6; ++i)
            {
                if((chefs[i].neededIng1==(*arrayAdd) || chefs[i].neededIng1==*(arrayAdd+sizeof(char))) && (chefs[i].neededIng2==(*arrayAdd) || chefs[i].neededIng2==*(arrayAdd+sizeof(char))))
                {
                    while(((returnVal=sem_post(&((*semAdd).chefSem[i]))))==-1 && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("Cannot post chef sem in pusher");
                        sem_post(&((*semAdd).chefSem[i]));
                    }
                    break;
                }
                else if(i==5)
                    while(((returnVal=sem_post(&((*semAdd).emptySem))))==-1 && errno==EINTR);
            }

            while(((returnVal=sem_post(&((*semAdd).critSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot post on critSem in pusher");

                (*endedAdd)=1;
                sem_post(&((*semAdd).emptySem));
                for(int i=0; i<6; ++i)
                {
                    while(((returnVal=sem_post(&((*semAdd).chefSem[i]))))==-1 && errno==EINTR);
                    if(returnVal==-1)
                    {
                        perror("sem post failed");
                        sem_post(&((*semAdd).chefSem[i]));
                    }
                }
                closeAll();
                exit(EXIT_FAILURE);

            }
        }

        closeAll();
        return 0;

    }

    //Opening the data file
    int dataFile=openFile(dataFilePath, O_RDONLY, 0555);
    if(dataFile<0)
    {
        perror("The data file cannot be opened");

        //Exit all chefs
        *endedAdd=1;
        while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
        for(int i=0; i<7; ++i)
            waitpid(-1, NULL, 0);

        closeAll();
        exit(EXIT_FAILURE);
    }

    //Wholesaler processing here
    char output1[200];
    int receivedDesserts=0;
    int status;
    int fail=0;
    while(true)
    {
        while(((returnVal=sem_wait(&((*semAdd).emptySem))))==-1 && errno==EINTR);
        if(returnVal==-1)
        {
            perror("Cannot wait on empty sem");

            //Exit all chefs
            *endedAdd=1;
            while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
            for(int i=0; i<7; ++i)
            {
                waitpid(-1, &status, 0);
                receivedDesserts+=WEXITSTATUS(status);
            }

            //Exit Output
            snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
            writeOutput(output1, STDOUT_FILENO);

            closeAll();
            close(dataFile);
            exit(EXIT_FAILURE);
        }

        if((*endedAdd)==-1)
            break;

        while(((returnVal=sem_wait(&((*semAdd).critSem))))==-1 && errno==EINTR);
        if(returnVal==-1)
        {
            perror("Cannot wait on critSem");

            //Exit all chefs
            *endedAdd=1;
            while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
            for(int i=0; i<7; ++i)
            {
                waitpid(-1, &status, 0);
                receivedDesserts+=WEXITSTATUS(status);
            }

            //Exit Output
            snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
            writeOutput(output1, STDOUT_FILENO);

            closeAll();
            close(dataFile);
            exit(EXIT_FAILURE);
        }

        //If this is not the first time the wholesaler is delivering something, it means it received a dessert
        if(*(endedAdd+sizeof(int))!=0 && fail!=1)
        {
            snprintf(output1, 200, "the wholesaler (pid %d) has obtained the dessert and left\n", getpid());
            if(writeOutput(output1, STDOUT_FILENO)==-1)
                perror("Write to stdout failed");
        }
        else if(fail==1)
        {
            snprintf(output1, 200, "the wholesaler (pid %d) failed to obtain dessert because no chef was interested in the ingredients\nThe interested chef might have already exited.\n", getpid());
            writeOutput(output1, STDOUT_FILENO);
            fail=0;
        }

        //Retrieving the ingredients
        char letters[2];
        int current=0;
        while(true)
        {
            char readChar[1];
            int bytesread;
            while(((bytesread=read(dataFile, readChar, 1))==-1) && (errno==EINTR));
            if(bytesread<0)
            {
                perror("Cannot read from the file");

                //Exit all chefs
                *endedAdd=1;
                while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
                for(int i=0; i<7; ++i)
                {
                    waitpid(-1, &status, 0);
                    receivedDesserts+=WEXITSTATUS(status);
                }

                //Exit Output
                snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
                writeOutput(output1, STDOUT_FILENO);

                close(dataFile);
                closeAll();
                exit(EXIT_FAILURE);
            }
            else if(bytesread==0)
            {
                *endedAdd=1;
                break;
            }
            else if(readChar[0]!='\n' && readChar[0]!='\r' && readChar[0]!='\t' && readChar[0]!=' ')
            {
                letters[current]=readChar[0];
                current++;
            }
            else if(readChar[0]=='\n' && current==2)
                break;
        }

        if(*endedAdd==1)
            break;

        //First output
        arrangeWholesalerOutput1(output1, getpid(), letters[0], letters[1]);
        if(writeOutput(output1, STDOUT_FILENO)==-1)
            perror("Cannot write to stdout");

        //Submiting the ingredients
        *arrayAdd=letters[0];
        *(arrayAdd+sizeof(char))=letters[1];

        //Wait output
        snprintf(output1, 200, "the wholesaler (pid %d) is waiting for the dessert\n", getpid());
        if(writeOutput(output1, STDOUT_FILENO)==-1)
            perror("Cannot write to stdout");

        for(int i=0; i<6; i++)
            if(((*forbiddenAdd).pair[i][0]==letters[0] || (*forbiddenAdd).pair[i][1]==letters[0]) && ((*forbiddenAdd).pair[i][0]==letters[1] || (*forbiddenAdd).pair[i][1]==letters[1]))
            {
                fail=1;
                while(((sem_post(&((*semAdd).emptySem))))==-1 && errno==EINTR);
                break;
            }

        while(((returnVal=sem_post(&((*semAdd).critSem))))==-1 && errno==EINTR);
        if(returnVal==-1)
        {
            perror("Cannot post on critSem");

            //Exit all chefs
            *endedAdd=1;
            while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
            for(int i=0; i<7; ++i)
            {
                waitpid(-1, &status, 0);
                receivedDesserts+=WEXITSTATUS(status);
            }

            //Exit Output
            snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
            writeOutput(output1, STDOUT_FILENO);

            close(dataFile);
            closeAll();
            exit(EXIT_FAILURE);
        }

        if(fail!=1)
        {
            while(((returnVal=sem_post(&((*semAdd).fullSem))))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot post on fullSem");

                //Exit all chefs
                *endedAdd=1;
                while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
                for(int i=0; i<7; ++i)
                {
                    waitpid(-1, &status, 0);
                    receivedDesserts+=WEXITSTATUS(status);
                }

                //Exit Output
                snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
                writeOutput(output1, STDOUT_FILENO);

                close(dataFile);
                closeAll();
                exit(EXIT_FAILURE);
            }
        }

    }

    while(((returnVal=sem_post(&((*semAdd).fullSem))))==-1 && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot post on fullSem");

        //Exit all chefs
        *endedAdd=1;
        while((sem_post(&((*semAdd).fullSem)))==-1 && errno==EINTR);
        for(int i=0; i<7; ++i)
        {
            waitpid(-1, &status, 0);
            receivedDesserts+=WEXITSTATUS(status);
        }

        //Exit Output
        snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
        writeOutput(output1, STDOUT_FILENO);

        close(dataFile);
        closeAll();
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<7; ++i)
    {
        waitpid(-1, &status, 0);
        receivedDesserts+=WEXITSTATUS(status);
    }

    //Exit Output
    snprintf(output1, 200, "the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), receivedDesserts);
    if(writeOutput(output1, STDOUT_FILENO)==-1)
        perror("Cannot write to stdout");

    close(dataFile);
    closeAll();

    return 0;
}

void checkArgc(int count)
{
    if(count!=3)
    {
        char errorMessage[]="You need exactly 3 command line arguments (first being auto inputed) to run the program. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], char** dataPath)
{
    int option;
    while((option=getopt(count, arguements, "i:"))!=-1)
        switch(option)
        {
            case 'i':
                *dataPath=optarg;
                break;
            default:
                return -1;
        }

    return 0;
}

int openFile(char* filePath, int flags, mode_t mode)
{
    int fd;
    if(mode!=0)
        while((fd=open(filePath, flags, mode))==-1 && errno==EINTR);
    else
        while((fd=open(filePath, flags))==-1 && errno==EINTR);

    return fd;
}

void arrangeWholesalerOutput1(char* output, int id, char ingLetter1, char ingLetter2)
{
    char* ingredient1;
    char* ingredient2;

    if(ingLetter1=='M')
        ingredient1="milk";
    else if(ingLetter1=='W')
        ingredient1="walnuts";
    else if(ingLetter1=='S')
        ingredient1="sugar";
    else
        ingredient1="flour";

    if(ingLetter2=='M')
        ingredient2="milk";
    else if(ingLetter2=='W')
        ingredient2="walnuts";
    else if(ingLetter2=='S')
        ingredient2="sugar";
    else
        ingredient2="flour";

    char buffer[200];
    snprintf(buffer, 200, "the wholesaler (pid %d) delivers %s and %s\n", id, ingredient1, ingredient2);
    strncpy(output, buffer, 200);
}

int writeOutput(char* output, int fileFd)
{
    int byteswritten=-1;
    //Locking the file for writing
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type=F_WRLCK;

    while(((fcntl(fileFd, F_SETLKW, &lock))==-1) && errno==EINTR);
    while(((byteswritten=write(fileFd, output, strlen(output)))==-1) && (errno==EINTR));

    //Unlocking the file
    lock.l_type=F_UNLCK;
    while(((fcntl(fileFd, F_SETLKW, &lock))==-1) && errno==EINTR);

    return byteswritten;
}

void arrangeChefOutput(char* output, int no, int id, char ingLetter1, char ingLetter2, char array1, char array2)
{
    char* ingredient1;
    char* ingredient2;

    if(ingLetter1=='M')
        ingredient1="milk";
    else if(ingLetter1=='W')
        ingredient1="walnuts";
    else if(ingLetter1=='S')
        ingredient1="sugar";
    else
        ingredient1="flour";

    if(ingLetter2=='M')
        ingredient2="milk";
    else if(ingLetter2=='W')
        ingredient2="walnuts";
    else if(ingLetter2=='S')
        ingredient2="sugar";
    else
        ingredient2="flour";

    char buffer[200];
    snprintf(buffer, 200, "chef%d (pid %d) is waiting for %s and %s, (%c%c)\n", no, id, ingredient1, ingredient2, array1, array2);
    strncpy(output, buffer, 200);
}

void arrangeChefOutput2(char* output, int no, int id, char ingLetter, char ingLetterOther, int count)
{
    char* ingredient1;

    if(ingLetter=='M')
        ingredient1="milk";
    else if(ingLetter=='W')
        ingredient1="walnuts";
    else if(ingLetter=='S')
        ingredient1="sugar";
    else
        ingredient1="flour";

    char buffer[200];

    if(count==1)
        snprintf(buffer, 200, "chef%d (pid %d) has taken the %s, (%c%c)\n", no, id, ingredient1, ingLetter, ingLetterOther);
    else
        snprintf(buffer, 200, "chef%d (pid %d) has taken the %s, (%c%c)\n", no, id, ingredient1, ingLetterOther, ingLetter);
    strncpy(output, buffer, 200);
}

void closeAll()
{
    shm_unlink("/EndedFlag");
    munmap("/EndedFlag", sizeof(int)*2);
    shm_unlink("/ArrayMem");
    munmap("/ArrayMem", sizeof(char)*2);
    shm_unlink("/ForbiddenMem");
    munmap("/ForbiddenMem", sizeof(struct forbiddenPairs));
    sem_close(&((*semAdd).critSem));
    sem_destroy(&((*semAdd).critSem));
    sem_close(&((*semAdd).emptySem));
    sem_destroy(&((*semAdd).emptySem));
    sem_close(&((*semAdd).fullSem));
    sem_destroy(&((*semAdd).fullSem));
    sem_close(&((*semAdd).chefSem[0]));
    sem_destroy(&((*semAdd).chefSem[0]));
    sem_close(&((*semAdd).chefSem[1]));
    sem_destroy(&((*semAdd).chefSem[1]));
    sem_close(&((*semAdd).chefSem[2]));
    sem_destroy(&((*semAdd).chefSem[2]));
    sem_close(&((*semAdd).chefSem[3]));
    sem_destroy(&((*semAdd).chefSem[3]));
    sem_close(&((*semAdd).chefSem[4]));
    sem_destroy(&((*semAdd).chefSem[4]));
    sem_close(&((*semAdd).chefSem[5]));
    sem_destroy(&((*semAdd).chefSem[5]));
    shm_unlink("/SemMem");
    munmap("/SemMem", sizeof(struct semSharedMem));
}
