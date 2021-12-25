#include <stdio.h>
#include <stdlib.h>

struct StudentMark{
    char name[20];
    char surname[20];
    int midterm;
    int finalp;
};

void GetStudentInfo(struct StudentMark* student, int studentAmount, FILE* fp)
{
    fread(student, sizeof(struct StudentMark)*studentAmount,1, fp);
}

void averageCalc(struct StudentMark* student, int studentAmount)
{
    int i;
    float storeAverage[studentAmount];

    for(i=0; i<studentAmount; i++)
    {
        storeAverage[i]=(0.4*(student[i].midterm))+(0.6*(student[i].finalp));
        printf("The student %s %s's midterm mark is %d, final mark is %d, and his/her average is %0.2f\n", student[i].name, student[i].surname, student[i].midterm, student[i].finalp, storeAverage[i]);
    }
}

int main()
{
    struct StudentMark* STUDENT_MARK=NULL;
    int studentAmount;

    FILE* fp;

    fp=fopen("marks_190704001.txt", "r");//can do with rb too, both works.
    if (NULL == fp)
    {
        fprintf(stderr, "Cannot open file for reading.\n");
        exit(1);
    }

    fread(&studentAmount, sizeof(int), 1, fp);

    STUDENT_MARK=(struct StudentMark*)malloc(sizeof(struct StudentMark)*studentAmount);
    if(NULL==STUDENT_MARK)
    {
        fprintf(stderr, "Cannot allocate memory.\n");
        exit(1);
    }

    GetStudentInfo(STUDENT_MARK, studentAmount, fp);

    averageCalc(STUDENT_MARK, studentAmount);

    free(STUDENT_MARK);
    fclose(fp);

    return 0;
}
