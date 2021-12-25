%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

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
[d][e][f][v][a][r]                  return KW_DEFVAR;
[f][o][r]                           return KW_FOR;
[w][h][i][l][e]                     return KW_WHILE;
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
[']								    return CUT;

[;][;].*                                        return COMMENT;

[a-zA-Z_][a-zA-Z0-9_]*                          {
                                                    yylval.stringVal = strdup(yytext);
                                                    return IDENTIFIER;
                                                }
[0-9]*[a-zA-Z_][a-zA-Z0-9_]*                    {yyerror("INVALID TOKEN\n");}
[0][.][0-9]+[a-zA-Z_][a-zA-Z0-9_]*              {yyerror("INVALID TOKEN\n");}
[1-9][0-9]*[.][0-9]+[a-zA-Z_][a-zA-Z0-9_]*      {yyerror("INVALID TOKEN\n");}

[0][0-9]+[.][0-9]+                              {yyerror("INVALID TOKEN\n");}
[0][0-9]+                                       {yyerror("INVALID TOKEN\n");}
[0]|[1-9][0-9]*                                 {
                                                    yylval.intVal = atoi(yytext);
                                                    return VALUE;
                                                }
[0][.][0-9]+|[1-9][0-9]*[.][0-9]+               {
                                                    yylval.intVal = atoi(yytext);
                                                    return VALUE;
                                                }

[\n]|[\r]                                       {;}
[ \t]+                                          {;}
[ ]                                             {;}
<<EOF>>                                         {;}
.                                               {yyerror("INVALID TOKEN\n");}

%%
