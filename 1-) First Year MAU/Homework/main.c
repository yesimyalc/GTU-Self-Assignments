#include <stdio.h>
#include <stdlib.h>

struct StudentMark{
    char name[20];
    char surname[20];
    int midterm;
    int finalp;
};

struct StudentMark studentInfo(struct StudentMark* student, int studentAmount)
{
    int i;
    for(i=0; i<studentAmount; i++)
    {
        printf("Enter the name of the student%d: ", i+1);
        scanf("%s", student[i].name);
        printf("Enter the surname of the student%d: ", i+1);
        scanf("%s", student[i].surname);
        printf("Enter the midterm point of student%d:  ", i+1);
        scanf("%d", &student[i].midterm);

        while(student[i].midterm<0 || student[i].midterm>100)
        {
            printf("You have entered an invalid midterm point, correct and enter again.\n");

            printf("Enter the midterm point of student%d:  ", i+1);
            scanf("%d", &student[i].midterm);
        }

        printf("Enter the final point of student%d: ", i+1);
        scanf("%d", &student[i].finalp);

        printf("\n");

        while(student[i].finalp<0 || student[i].finalp>100)
        {
            printf("You have entered an invalid final point, correct and enter again.");

            printf("Enter the final point of student%d:  ", i+1);
            scanf("%d", &student[i].finalp);
        }
    }

    return *student;
}

void StoreStudentInfo(struct StudentMark* student, int studentAmount, FILE* fp)
{
    fwrite(&studentAmount, sizeof(int),1,fp);
    fwrite(student, sizeof(struct StudentMark)*studentAmount,1, fp);
}

int main()
{
    struct StudentMark* STUDENT_MARK=NULL;
    int studentAmount;

    printf("How many student information do you want to store?\n");
    scanf("%d", &studentAmount);

    STUDENT_MARK=(struct StudentMark*)malloc(sizeof(struct StudentMark)*studentAmount);
    if(NULL==STUDENT_MARK)
    {
        fprintf(stderr, "Cannot allocate memory.\n");
        exit(1);
    }

    *STUDENT_MARK=studentInfo(STUDENT_MARK, studentAmount);

    FILE* fp;

    fp=fopen("marks_190704001.txt", "w");//can do with wb too, both works
    if (NULL == fp)
    {
        fprintf(stderr, "Cannot open file for writing.\n");
        exit(1);
    }

    StoreStudentInfo(STUDENT_MARK, studentAmount, fp);

    free(STUDENT_MARK);
    fclose(fp);

    return 0;
}
