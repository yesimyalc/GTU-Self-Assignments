lex gpp_lexer.l 
yacc gpp_interpreter.y -d
gcc lex.yy.c y.tab.c -w
./a.out