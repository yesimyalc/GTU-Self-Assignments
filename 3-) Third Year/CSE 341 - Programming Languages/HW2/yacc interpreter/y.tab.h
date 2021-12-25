/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KW_AND = 258,
    KW_OR = 259,
    KW_NOT = 260,
    KW_EQUAL = 261,
    KW_LESS = 262,
    KW_NIL = 263,
    KW_LIST = 264,
    KW_APPEND = 265,
    KW_CONCAT = 266,
    KW_SET = 267,
    KW_DEFFUN = 268,
    KW_DEFVAR = 269,
    KW_FOR = 270,
    KW_WHILE = 271,
    KW_IF = 272,
    KW_EXIT = 273,
    KW_LOAD = 274,
    KW_DISP = 275,
    KW_TRUE = 276,
    KW_FALSE = 277,
    OP_PLUS = 278,
    OP_MINUS = 279,
    OP_DIV = 280,
    OP_DBLMULT = 281,
    OP_MULT = 282,
    OP_COMMA = 283,
    OP_OP = 284,
    OP_CP = 285,
    OP_OC = 286,
    OP_CC = 287,
    COMMENT = 288,
    CUT = 289,
    IDENTIFIER = 290,
    VALUE = 291
  };
#endif
/* Tokens.  */
#define KW_AND 258
#define KW_OR 259
#define KW_NOT 260
#define KW_EQUAL 261
#define KW_LESS 262
#define KW_NIL 263
#define KW_LIST 264
#define KW_APPEND 265
#define KW_CONCAT 266
#define KW_SET 267
#define KW_DEFFUN 268
#define KW_DEFVAR 269
#define KW_FOR 270
#define KW_WHILE 271
#define KW_IF 272
#define KW_EXIT 273
#define KW_LOAD 274
#define KW_DISP 275
#define KW_TRUE 276
#define KW_FALSE 277
#define OP_PLUS 278
#define OP_MINUS 279
#define OP_DIV 280
#define OP_DBLMULT 281
#define OP_MULT 282
#define OP_COMMA 283
#define OP_OP 284
#define OP_CP 285
#define OP_OC 286
#define OP_CC 287
#define COMMENT 288
#define CUT 289
#define IDENTIFIER 290
#define VALUE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 30 "gpp_interpreter.y" /* yacc.c:1909  */

    int intVal;
    char* stringVal;
    int values[1000];   //Values in a line

#line 132 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
