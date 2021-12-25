%{
#include <stdio.h>
#include <string.h>

//All Definitions
#define KW_AND                            		1
#define KW_OR                       			2
#define KW_NOT                             		3
#define KW_EQUAL                           		4
#define KW_LESS                           		5
#define KW_NIL                             		6
#define KW_LIST			                        7
#define KW_APPEND                          		8
#define KW_CONCAT                         		9
#define KW_SET                        			10
#define KW_DEFFUN                           	11
#define KW_FOR                             		12
#define KW_IF                            		13
#define KW_EXIT                            		14
#define KW_LOAD                          		15
#define KW_DISP									16
#define KW_TRUE									17
#define KW_FALSE								18

#define OP_PLUS									19
#define OP_MINUS 								20
#define OP_DIV 									21
#define OP_MULT 								22
#define OP_DBLMULT 								23
#define OP_COMMA 								24
#define OP_OP 									25
#define OP_CP 									26
#define OP_OC 									27
#define OP_CC									28

#define COMMENT 								29
#define IDENTIFIER 								30
#define VALUE 		   							31

#define NEWLINE 								32
#define SPACE								    33
#define END                                     34

#define UNKNOWN									35

#define NO_ERROR                                0
#define LIST_ERROR                              1

int op_oc_count=0; //Saves the " count to determine if " is OP_OC or OP_CC
%}

%%

[a][n][d]                           return KW_AND;
[o][r]                              return KW_OR;
[n][o][t]                           return KW_NOT;
[e][q][u][a][l]                     return KW_EQUAL;
[l][e][s][s]                        return KW_LESS;
[n][i][l]                           return KW_NIL;
[l][i][s][t]                        return KW_LIST;
[a][p][p][e][n][d]                  return KW_APPEND;
[c][o][n][c][a][t]                  return KW_CONCAT;
[s][e][t]                           return KW_SET;
[d][e][f][f][u][n]                  return KW_DEFFUN;
[f][o][r]                           return KW_FOR;
[i][f]                              return KW_IF;
[e][x][i][t]                        return KW_EXIT;
[l][o][a][d]                        return KW_LOAD;
[d][i][s][p]                        return KW_DISP;
[t][r][u][e]                        return KW_TRUE;
[f][a][l][s][e]                     return KW_FALSE;

[+]                                 return OP_PLUS;
[-]                                 return OP_MINUS;
[/]                                 return OP_DIV;
[*][*]                              return OP_DBLMULT;
[*]                                 return OP_MULT;
[,]                                 return OP_COMMA;
[(]                                 return OP_OP;
[)]                                 return OP_CP;
["]                                 {
                                        if(op_oc_count==0)
                                        {
                                            op_oc_count++;
                                            return OP_OC;
                                        }
                                        else
                                        {
                                            op_oc_count=0;
                                            return OP_CC;
                                        }
                                    }

[;][;].*                                        return COMMENT;

[a-zA-Z_][a-zA-Z0-9_]*                          return IDENTIFIER;
[0-9]*[a-zA-Z_][a-zA-Z0-9_]*                    return UNKNOWN;
[0][.][0-9]+[a-zA-Z_][a-zA-Z0-9_]*              return UNKNOWN;
[1-9][0-9]*[.][0-9]+[a-zA-Z_][a-zA-Z0-9_]*      return UNKNOWN;

[0][0-9]+[.][0-9]+                              return UNKNOWN;
[0][0-9]+                                       return UNKNOWN;
[0]|[1-9][0-9]*                                 return VALUE;
[0][.][0-9]+|[1-9][0-9]*[.][0-9]+               return VALUE;

[\n]|[\r]                                       return NEWLINE;
[ \t]+                                          return SPACE;
<<EOF>>                                         return END;
.                                               return UNKNOWN;

%%

int yywrap(){}
void setTempToken(int tempToken, int* tempArraySize, int* tempArray);
void clearTempTokenArray(int* tempArraySize, int* tempArray);
int checkTokensSyntacs(int* tempArraySize, int* tempArray);

int main()
{
    const char tokens[][20]={
        "", "KW_AND", "KW_OR", "KW_NOT", "KW_EQUAL", "KW_LESS", "KW_NIL", "KW_LIST", "KW_APPEND", "KW_CONCAT", "KW_SET", "KW_DEFFUN",
        "KW_FOR", "KW_IF", "KW_EXIT", "KW_LOAD", "KW_DISP", "KW_TRUE", "KW_FALSE", "OP_PLUS", "OP_MINUS", "OP_DIV", "OP_MULT",
        "OP_DBLMULT", "OP_COMMA", "OP_OP", "OP_CP", "OP_OC", "OP_CC", "COMMENT", "IDENTIFIER", "VALUE", "NEWLINE", "SPACE",
        "END", "UNKNOWN"
    };
    const char checks[][50]={
        "NO_ERROR", "LIST_ERROR"
    };
    FILE* fread;
    FILE* fp;

    fp=fopen("output.txt", "w+");
    if (NULL == fp)
    {
        fprintf(stderr, "Cannot open file for writing.\n");
        exit(1);
    }

    int token_no=1;
    int temp=1;
    char tempres[240];
    int token_temp[240];
    int temp_size;
    char format_input[240];
    int format=0;

    printf("If you wanna read from a file enter filename. Else enter start.\n");
    printf("$g++ ");
    scanf("%s", format_input);

    if(strcmp(format_input, "start")!=0)
    {
        format=1;
        fread=fopen(format_input, "r");
        if (NULL == fread)
        {
            fprintf(stderr, "Cannot open file for reading.\n");
            exit(1);
        }
        yyin=fread;
    }

    while(token_no!=KW_EXIT && token_no!=UNKNOWN && token_no!=END)
    {
        clearTempTokenArray(&temp_size, token_temp);

        if(format==0)
            printf("\n>");
	else
        	printf("\n");

        token_no=yylex();
        setTempToken(token_no, &temp_size, token_temp);

        while(token_no!=NEWLINE && token_no!=KW_EXIT && token_no!=END)
        {
            //Checks for exact unknown characters
            if(token_no==UNKNOWN)
            {
               printf("SYNTAX ERROR: %s is found. %s cannot be tokenized.\n", tokens[token_no], yytext);
               fprintf(fp,"SYNTAX ERROR: %s is found. %s cannot be tokenized.\n", tokens[token_no], yytext);
               token_no=UNKNOWN;
               break;
            }

            if(token_no!=NEWLINE) //Check for some extra lexical errors
            {
                strcpy(tempres, yytext);
                temp = yylex();

                if(token_no >= KW_AND && token_no <= KW_FALSE)
                {
                    if(temp != SPACE && temp != OP_OP && temp != NEWLINE && temp!=OP_OC && temp!=IDENTIFIER && temp!=VALUE && temp != OP_CP && temp != OP_CC)
                    {
                        printf("SYNTAX ERROR: %s%s can not be tokenized.\n", tempres, yytext);
                        fprintf(fp,"SYNTAX ERROR: %s%s can not be tokenized.\n", tempres, yytext);
                        token_no = UNKNOWN;
                        break;
                    }

                }
                else if(token_no >= OP_PLUS && token_no <= OP_DBLMULT)
                {
                    if(temp != SPACE && temp != OP_OP && temp != NEWLINE && temp!=OP_OC && temp!=IDENTIFIER && temp!=VALUE)
                    {
                        printf("SYNTAX ERROR: %s%s can not be tokenized.\n", tempres, yytext);
                        fprintf(fp,"SYNTAX ERROR: %s%s can not be tokenized.\n", tempres, yytext);
                        token_no = UNKNOWN;
                        break;
                    }
                }

                if(token_no!=SPACE)
                {
                    printf("%s\n", tokens[token_no]);
                    fprintf(fp, "%s\n", tokens[token_no]);
                }

            }

            token_no=temp;
            setTempToken(token_no, &temp_size, token_temp);

            if(token_no==NEWLINE) //Checks if all the input is given
            {
                //Checks for some syntactic errors. I have only implemented the list errors. This was for practice. It was not needed for the homework.
                int check=checkTokensSyntacs(&temp_size, token_temp);
                if(check==1)
                {
                    printf("SYNTAX ERROR: ");
                    fprintf(fp,"SYNTAX ERROR: ");
                    int i;
                    for(i=0; i<temp_size; ++i)
                        printf("%s ", tokens[token_temp[i]]);
                    printf("is not an allowed statement in G++ language due to %s.\n", checks[check]);
                    fprintf(fp,"is not an allowed statement in G++ language due to %s.\n", checks[check]);
                    token_no=UNKNOWN;
                    break;
                }
                else if(check==0) //If no lexical or syntactic errors are found, print the found tokens
                {
                    fprintf(fp, "\n");
                }
            }

        }

    }

    if(token_no==KW_EXIT)
    {
        printf("%s\n", tokens[token_no]);
        fprintf(fp, "%s\n", tokens[token_no]);
        printf("exit command is read. The lexer is closing.");
    }
    else if(token_no==UNKNOWN)
        printf("The lexer is closing due to an ERROR.");

    fclose(fp);

    if(format==1)
        fclose(fread);

    return 0;
}

void setTempToken(int tempToken, int* tempArraySize, int* tempArray)
{
    if(tempToken!=SPACE && tempToken!=UNKNOWN && tempToken!=KW_EXIT && tempToken!=NEWLINE)
    {
        *(tempArray+*tempArraySize)=tempToken;
        (*tempArraySize)++;
    }
}

void clearTempTokenArray(int* tempArraySize, int* tempArray)
{
    int i;
    for(i=0; i<*tempArraySize; ++i)
        *(tempArray+*tempArraySize)=0;
    (*tempArraySize)=0;
}

int checkTokensSyntacs(int* tempArraySize, int* tempArray)
{
    //List Check
    if(*tempArraySize>=3 && tempArray[0]==OP_OP && tempArray[1]==KW_LIST)
    {
        int i;
        for(i=2; i<*tempArray; ++i)
        {
            if(tempArray[i]==OP_COMMA && tempArray[i-1]!=VALUE) //check for value before comma
                return 1;
            else if(tempArray[i]==OP_COMMA && i+1>=*tempArraySize) //check if comma is not at the end
                return 1;
            else if(tempArray[i]==OP_COMMA && tempArray[i+1]!=VALUE) //check if value after comma
                return 1;
            else if(tempArray[i]!=VALUE && tempArray[i]!=OP_CP && tempArray[i]!=OP_COMMA) //check if there is something different than value, ) or comma
                return 1;
            else if(tempArray[i]!=OP_CP && i==(*tempArraySize)-1) //check if no ) is found.
                return 1;

            if(tempArray[i]==OP_CP) //if ) is found it is a proper list
                break;
        }

    }

    return 0;

}
