%{
    #include <stdio.h>
    #include <string.h>

    int lineCount=1;
    int valueCount=0; //Value count in a line
    int valueCount2=0;
    int totalValueCount=0;
    int errorFlag=0;
    int flag=-1;   //0=number 1=string 3=list
    int format=0;
    FILE* fp;

    struct var //Storing all variables with their values
	{
	   char identifiers[1000][200];
	   int values[1000];
	   int count;
	};

	struct var variables = {"", 0, 0, 0}; //Initializing variables

	void copyArray(int array1[], int array2[], int size);
	int isInVariables(char *my_str, char string_list[1000][200], int num_strings);
	int yywrap();
	int yyerror(char *msg);
%}

%union
{
    int intVal;
    char* stringVal;
    int values[1000];   //Values in a line
}

%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT KW_SET
%token KW_DEFFUN KW_DEFVAR KW_FOR KW_WHILE KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE
%token OP_PLUS OP_MINUS OP_DIV OP_DBLMULT OP_MULT OP_COMMA OP_OP OP_CP OP_OC OP_CC
%token COMMENT CUT
%token IDENTIFIER
%token VALUE

%start START

%%

START: | START INPUT                    {
                                            printf("[%d]> ", ++lineCount);
                                            valueCount=0;
                                            valueCount2=0;
                                            totalValueCount=0;
                                            errorFlag=0;
                                            flag=-1;
                                        };

INPUT:
		EXPI                            {
                                            if(errorFlag==1){}
                                            else if(flag == 0)
                                            {
                                                printf("SYNTAX OK!\n");
                                                printf("%d\n", $<intVal>1);
                                            }
                                            else if(flag == 1)
                                            {
                                                printf("SYNTAX OK!\n");
                                                printf("%s\n", $<stringVal>1);
                                            }
                                            else if(flag== 3)
                                            {
                                                int i;
                                                printf("SYNTAX OK!\n");
                                                printf("(");
                                                for(i = 0; i < valueCount2; i ++)
                                                {
						    if($<values>1[i]!=0)
						    {
                                                    	printf("%d", $<values>1[i]);
		                                        if(i != valueCount-1)
		                                           printf(" ");
						    }
                                                }
                                                 printf(")\n");
                                            }
                                        }
		| EXPLISTI                      {
                                            if(errorFlag==1){}
                                            else if(flag == 0)
                                            {
                                                printf("SYNTAX OK!\n");
                                                printf("%d\n", $<intVal>1);
                                            }
                                            else if(flag == 1)
                                            {
                                                printf("SYNTAX OK!\n");
                                                printf("%s\n", $<stringVal>1);
                                            }
                                            else if(flag == 3)
                                            {
                                                int i;
                                                printf("SYNTAX OK!\n");
                                                printf("(");
                                                for(i = 0; i < valueCount2; i ++)
                                                {
                                                    if($<values>1[i]!=0)
						    {
                                                    	printf("%d", $<values>1[i]);
		                                        if(i != valueCount-1)
		                                           printf(" ");
						    }
                                                }
                                                 printf(")\n");
                                            }
                                        }
		| COMMENT                       {
                                            printf("SYNTAX OK!\n");
                                            printf("COMMENT\n");
                                        };

EXPI:
		OP_OP OP_PLUS EXPI EXPI OP_CP       {
                                                if(errorFlag!=1)
                                                {
                                                    $<intVal>$ = $<intVal>3 + $<intVal>4;
                                                    flag = 0;
                                                }
                                            }
		| OP_OP OP_MINUS EXPI EXPI OP_CP    {
                                                if(errorFlag!=1)
                                                {
                                                   $<intVal>$ = $<intVal>3 - $<intVal>4;
                                                    flag = 0;
                                                }
                                            }
		| OP_OP OP_MULT EXPI EXPI OP_CP     {
                                                if(errorFlag!=1)
                                                {
                                                   $<intVal>$ = $<intVal>3 * $<intVal>4;
                                                    flag = 0;
                                                }
                                            }
		| OP_OP OP_DIV EXPI EXPI OP_CP      {
                                                if(errorFlag!=1)
                                                {
                                                   if($<intVal>4 != 0)
                                                    {
                                                        $<intVal>$ = $<intVal>3 / $<intVal>4;
                                                        flag = 0;
                                                    }
                                                    else
                                                    {
                                                        printf("Can not divide by zero\n");
                                                        errorFlag=1;
                                                    }
                                                }
                                            }
		| OP_OP OP_DBLMULT EXPI EXPI OP_CP  {
                                                if(errorFlag!=1)
                                                {
                                                    int x = $<intVal>3;
                                                    int i;
                                                    for(i = 1; i<$<intVal>4; i++)
                                                        x*=$<intVal>3;
                                                    $<intVal>$ = x;
                                                    flag = 0;
                                                }
										    }
		| OP_OP KW_SET IDENTIFIER EXPI OP_CP{
                                                if(errorFlag!=1)
                                                {
                                                    int index= isInVariables($<stringVal>3, variables.identifiers, variables.count);
                                                    if(flag==0)
                                                    {
                                                        if(index != -1)
                                                            variables.values[index]=$<intVal>4; //identifier has a value
                                                        else
                                                        {
                                                            strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                            variables.values[variables.count] = $<intVal>4;
                                                            variables.count += 1;
                                                        }
                                                        $<intVal>$ = $<intVal>4;
                                                        flag = 0;
                                                    }
                                                    else
                                                    {
                                                        int x;
                                                        if($<stringVal>4=="FALSE")
                                                           x=0;
                                                        else
                                                            x=1;
                                                        if(index != -1)
                                                            variables.values[index]=x; //identifier has a value
                                                        else
                                                        {
                                                            strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                            variables.values[variables.count] = x;
                                                            variables.count += 1;
                                                        }
                                                        $<intVal>$ = x;
                                                        flag = 0;
                                                    }
                                                }
											}
		| OP_OP KW_IF EXPB EXPLISTI OP_CP       {
                                                    if(errorFlag!=1)
                                                    {
                                                        if($<stringVal>3 == "TRUE" || $<intVal>3==1)
                                                        {
                                                            copyArray($<values>$, $<values>4, valueCount2);
                                                            flag = 3;
                                                        }
                                                        else
                                                        {
                                                            $<intVal>$ = 0;
                                                            flag = 3;
                                                        }
                                                    }
                                                }
		| OP_OP KW_IF EXPB EXPLISTI EXPLISTI OP_CP{
                                                    if(errorFlag!=1)
                                                    {
                                                        if($<stringVal>3 == "TRUE" || $<intVal>3==1)
                                                        {
                                                            copyArray($<values>$, $<values>4, valueCount2);
                                                            flag = 3;
                                                        }
                                                        else
                                                        {
                                                            copyArray($<values>$, $<values>5, valueCount2);
                                                            flag = 3;
                                                        }
                                                    }
												 }
		| OP_OP KW_IF EXPB EXPI OP_CP            {
                                                    if(errorFlag!=1)
                                                    {
                                                        if($<stringVal>3 == "TRUE" || $<intVal>3==1)
                                                        {
                                                            $<intVal>$ = $<intVal>4;
                                                            flag = 0;
                                                        }
                                                        else
                                                        {
                                                            $<intVal>$ = 0;
                                                            flag = 3;
                                                        }
                                                    }
                                                 }
		| OP_OP KW_IF EXPB EXPI EXPI OP_CP       {
                                                    if(errorFlag!=1)
                                                    {
                                                        if($<stringVal>3 == "TRUE" || $<intVal>3==1)
                                                        {
                                                            $<intVal>$ = $<intVal>4;;
                                                            flag = 0;
                                                        }
                                                        else
                                                        {
                                                            $<intVal>$ = $<intVal>5;
                                                            flag = 0;
                                                        }
                                                    }
                                                 }
		| OP_OP KW_WHILE OP_OP EXPB OP_CP EXPLISTI OP_CP {
                                                            if(errorFlag!=1)
                                                            {
                                                                if($<stringVal>4 == "TRUE" || $<intVal>4==1)
                                                                {
                                                                    copyArray($<values>$, $<values>6, valueCount2);
                                                                    flag = 3;
                                                                }
                                                                else
                                                                {
                                                                    $<stringVal>$=" ";
                                                                    flag=1;
                                                                }
                                                            }
                                                         }
		| OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP     {
                                                        if(errorFlag!=1)
                                                        {
                                                            strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                            if(flag==0)
                                                                variables.values[variables.count] = $<intVal>4;
                                                            else if($<stringVal>4=="FALSE")
                                                                variables.values[variables.count] = 0;
                                                            else
                                                                variables.values[variables.count] = 1;
                                                            variables.count += 1;
                                                            $<stringVal>$ = $<stringVal>3;
                                                            flag = 1;
                                                        }
                                                    }

		| OP_OP KW_DEFFUN IDENTIFIER OP_OP IDENTIFIERS OP_CP EXPI OP_CP {
                                                                            if(errorFlag!=1)
                                                                            {
                                                                                strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                                                variables.values[variables.count] = -1;
                                                                                variables.count += 1;
                                                                                $<stringVal>$ = $<stringVal>3;
                                                                                flag = 1;
                                                                            }
																		}
		| OP_OP KW_DEFFUN IDENTIFIER OP_OP IDENTIFIERS OP_CP EXPLISTI OP_CP {
                                                                                if(errorFlag!=1)
                                                                                {
                                                                                    strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                                                    variables.values[variables.count] = -1;
                                                                                    variables.count += 1;
                                                                                    $<stringVal>$ = $<stringVal>3;
                                                                                    flag = 1;
                                                                                }
																			}
		| OP_OP KW_DEFFUN IDENTIFIER EXPLISTI OP_CP {
                                                        if(errorFlag!=1)
                                                        {
                                                            strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                            variables.values[variables.count] = -1;
                                                            variables.count += 1;
                                                            $<stringVal>$ = $<stringVal>3;
                                                            flag = 1;
                                                        }
													}
        | OP_OP KW_DEFFUN IDENTIFIER EXPI OP_CP {
                                                        if(errorFlag!=1)
                                                        {
                                                            strcpy(variables.identifiers[variables.count], $<stringVal>3);
                                                            variables.values[variables.count] = -1;
                                                            variables.count += 1;
                                                            $<stringVal>$ = $<stringVal>3;
                                                            flag = 1;
                                                        }
													}
		| OP_OP KW_EXIT OP_CP                       {
                                                        printf("Interpreter exits.\n");
                                                        exit(1);
                                                    }
		| OP_OP KW_DISP EXPI OP_CP                  {
                                                        if(errorFlag!=1)
                                                        {
                                                            if(flag==0)
                                                            {
                                                                $<intVal>$=$<intVal>3;
                                                                flag=0;
                                                            }
                                                            else if(flag==1)
                                                            {
                                                                $<stringVal>$=$<stringVal>3;
                                                                flag=1;
                                                            }
                                                            else if(flag==3)
                                                            {
                                                                copyArray($<values>$, $<values>3, valueCount2);
                                                                flag=3;
                                                            }
                                                        }
                                                    }
		| OP_OP KW_DISP EXPLISTI OP_CP              {
                                                        if(errorFlag!=1)
                                                        {
                                                           copyArray($<values>$, $<values>3, valueCount2);
                                                           flag=3;
                                                        }
                                                    }
		| EXPB                                      {
                                                        if(errorFlag!=1)
                                                        {
                                                            if(flag==1)
                                                            {
                                                                $<stringVal>$=$<stringVal>1;
                                                                flag=1;
                                                            }
                                                            else
                                                            {
                                                                $<intVal>$=$<intVal>1;
                                                                flag=0;
                                                            }
                                                        }
                                                    };

EXPLISTI:
        OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP     {
                                                        if(errorFlag!=1)
                                                        {
                                                            int i;
                                                            int j = 0;
                                                            for(i=totalValueCount-valueCount2; i<totalValueCount; i++, j++)
                                                                $<values>3[i]=$<values>4[j];
                                                            copyArray($<values>$, $<values>3, totalValueCount);
                                                            valueCount2=totalValueCount;
                                                            flag=3;
                                                        }
                                                    }
		| OP_OP KW_APPEND EXPI EXPLISTI OP_CP       {
                                                        if(errorFlag!=1)
                                                        {
                                                            $<values>$[0] = $<intVal>3;
                                                            int i;
                                                            for(i=0; i<valueCount2; i++)
                                                                $<values>$[i+1] = $<values>4[i];
                                                            valueCount2+=1;
                                                            flag=3;
                                                        }
                                                    }
		| LISTVALUE                                 {
                                                        if(errorFlag!=1)
                                                        {
                                                            copyArray($<values>$, $<values>1, valueCount);
                                                            valueCount2=valueCount;
                                                            totalValueCount+=valueCount;
            						    valueCount=0;
                                                            flag=3;
                                                        }
                                                    };


EXPB:
		OP_OP KW_AND EXPB EXPB OP_CP        {
                                                if(errorFlag!=1)
                                                {
                                                   if(($<intVal>3 && $<intVal>4) || ($<stringVal>3=="TRUE" && $<stringVal>4=="TRUE"))
                                                        $<stringVal>$ = "TRUE";
                                                   else
                                                        $<stringVal>$ = "FALSE";
                                                   flag = 1;
                                                }
                                            }
		| OP_OP KW_OR EXPB EXPB OP_CP       {
                                                if(errorFlag!=1)
                                                {
                                                   if(($<intVal>3 || $<intVal>4) || ($<stringVal>3=="TRUE" && $<stringVal>4=="TRUE")
							|| ($<stringVal>3=="TRUE" && $<stringVal>4=="FALSE") || ($<stringVal>3=="FALSE" && $<stringVal>4=="TRUE"))
                                                     $<stringVal>$ = "TRUE";
                                                    else
                                                        $<stringVal>$ = "FALSE";
                                                    flag = 1;
                                                }
                                            }
		| OP_OP KW_NOT EXPB OP_CP           {
                                                if(errorFlag!=1)
                                                {
                                                  if(($<intVal>3 == 0) || ($<stringVal>3 == "FALSE"))
                                                    $<stringVal>$ = "TRUE";
                                                  else
                                                    $<stringVal>$ = "FALSE";
                                                  flag = 1;
                                                }
                                            }
		| OP_OP KW_EQUAL EXPB EXPB OP_CP    {
                                                if(errorFlag!=1)
                                                {
                                                    if(($<intVal>3 == $<intVal>4) || ($<stringVal>3 == $<stringVal>4))
                                                        $<stringVal>$ = "FALSE";
                                                    else
                                                        $<stringVal>$ = "TRUE";
                                                    flag = 1;
                                                }
                                            }
		| OP_OP KW_LESS EXPB EXPB OP_CP     {
                                                if(errorFlag!=1)
                                                {
                                                    if($<intVal>3 < $<intVal>4)
                                                        $<stringVal>$ = "TRUE";
                                                    else
                                                        $<stringVal>$ = "FALSE";
                                                    flag = 1;
                                                }
                                            }
		| VALUE                             {
                                                if(errorFlag!=1)
                                                {
                                                    $<intVal>$ = $<values>1[valueCount];
                                                    flag= 0;
                                                }
                                            }
		| KW_TRUE                           {
                                                if(errorFlag!=1)
                                                {
                                                    $<stringVal>$ = "TRUE";
                                                    flag = 1;
                                                }
                                            }
		| KW_FALSE                          {
                                                if(errorFlag!=1)
                                                {
                                                    $<stringVal>$ = "FALSE";
                                                    flag = 1;
                                                }
                                            }
		| KW_NIL                            {
                                                if(errorFlag!=1)
                                                {
                                                    $<stringVal>$ = "NILL";
                                                    flag = 1;
                                                }
                                            }
		| IDENTIFIER                        {
                                                if(errorFlag!=1)
                                                {
                                                    int index= isInVariables($<stringVal>1, variables.identifiers, variables.count);
                                                    if(index != -1)
                                                    {
                                                        $<intVal>$ = variables.values[index]; //identifier has a value
                                                        flag=0;
                                                    }
                                                    else
                                                    {
                                                        $<stringVal>$ = $<stringVal>1; //identifier has a value
                                                        flag=1;
                                                    }
                                                }
                                             };

LISTVALUE:
		OP_OP KW_LIST VALUES OP_CP      {
                                            if(errorFlag!=1)
                                            {
                                                copyArray($<values>$, $<values>3, valueCount);
                                                flag = 3;
                                            }
                                        }
		| CUT OP_OP VALUES OP_CP        {
                                            if(errorFlag!=1)
                                            {
                                                copyArray($<values>$, $<values>3, valueCount);
                                                flag = 3;
                                            }
                                        }
        | CUT OP_OP OP_CP               {
                                            if(errorFlag!=1)
                                            {
                                                $<stringVal>$="NULL LIST";
                                                flag = 1;
                                            }
                                        }

VALUES:
		VALUES VALUE                    {
                                            if(errorFlag!=1)
                                            {
                                                $<values>$[valueCount] = $<intVal>2; //adding value to values
                                                valueCount += 1;
                                                flag = 3;
                                            }
                                        }
		| VALUES IDENTIFIER             {
                                            if(errorFlag!=1)
                                            {
                                               int index= isInVariables($<stringVal>2, variables.identifiers, variables.count);
                                                if(index != -1)
                                                    $<intVal>2 = variables.values[index]; //identifier has a value
                                                else
                                                {
                                                    printf("ERROR: variable %s has no value\n", $<stringVal>2);
                                                    return;
                                                }
                                                $<values>$[valueCount] = $<intVal>2; //Saving variable value to values
                                                valueCount += 1;
                                                flag = 3;
                                            }
                                        }
        | IDENTIFIER                    {
                                            if(errorFlag!=1)
                                            {
                                               int index= isInVariables($<stringVal>1, variables.identifiers, variables.count);
                                                if(index != -1)
                                                    $<intVal>1 = variables.values[index]; //identifier has a value
                                                else
                                                {
                                                    printf("ERROR: variable %s has no value\n", $<stringVal>1);
                                                    return;
                                                }
                                                $<values>$[valueCount] = $<intVal>1; //Saving variable value to values
                                                valueCount += 1;
                                                flag = 3;
                                            }
                                        }
		| VALUE                         {
                                            if(errorFlag!=1)
                                            {
                                                $<values>$[valueCount] = $<intVal>1; //Saving variable value to values
                                                valueCount+=1;
                                                flag = 3;;
                                            }
                                        };

IDENTIFIERS:
		IDENTIFIERS IDENTIFIER          {
                                            if(errorFlag!=1)
                                            {
                                                int index = isInVariables($<stringVal>2, variables.identifiers, variables.count);
                                                if(index == -1)
                                                {
                                                    strcpy(variables.identifiers[variables.count], $<stringVal>2);  //save identifer to identifiers array
                                                    variables.values[variables.count]=-1; //Initialize its assigned value as -1
                                                    variables.count += 1;
                                                }

                                                $<stringVal>$ = $<stringVal>1;
                                                strcat($<stringVal>$, " ");
                                                strcat($<stringVal>$, $<stringVal>2); //concat two identifiers
                                                flag = 1;
                                            }
                                        }
		| IDENTIFIER                    {
                                            if(errorFlag!=1)
                                            {
                                                int index = isInVariables($<stringVal>1, variables.identifiers, variables.count);
                                                if(index == -1)
                                                {
                                                    strcpy(variables.identifiers[variables.count], $<stringVal>1);  //save identifer to identifiers array
                                                    variables.values[variables.count]=-1; //Initialize its assigned value as -1
                                                    variables.count += 1;
                                                }
                                                $<stringVal>$ = $<stringVal>1;
                                                flag = 1;
                                            }
                                        };

%%

int yyerror(char *msg)
{
  printf("SYNTAX ERROR: Expression not recognized\n");
  printf("[%d]> ", ++lineCount);
  yylex_destroy();
  valueCount=0;
  valueCount2=0;
  totalValueCount=0;
  errorFlag=0;
  flag=-1;
  if(format==1)
    exit(0);
  else
    yyset_in(stdin);
  yyparse();
}

int yywrap(){
	return 1;
}

int isInVariables(char *my_str, char string_list[1000][200], int num_strings)
{
    int i;
    for(i = 0; i < num_strings; i++ )
        if (strcmp(my_str, string_list[i]) == 0 )
            return i;

    return -1;
}

void copyArray(int array1[], int array2[], int size)
{
    int i;
    for(i = 0; i < size; i++)
        array1[i] = array2[i];
}

int main(void)
{
    char format_input[240];

    printf("If you wanna read from a file enter filename. Else enter start.\n");
    printf("$g++ ");
    scanf("%s", format_input);

    if(strcmp(format_input, "start")!=0)
    {
        format=1;
        fp=fopen(format_input, "r");
        if (NULL == fp)
        {
            fprintf(stderr, "Cannot open file for reading.\n");
            exit(1);
        }
        yyset_in(fp);
    }
    else
    {
        yyset_in(stdin);
        printf("[1]> ");
    }

  	yyparse();
  	if(format==1)
        fclose(fp);
}
