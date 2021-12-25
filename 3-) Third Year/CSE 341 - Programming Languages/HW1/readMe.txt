1-) Flex Lexer
-You can use the makefile to compile the flex lexer.
-This lexer can work with both files and regular user input.
-The directions to choose a file or user input is explained in the program.
-Supports floating numbers as mentioned in g++ syntax.

*********
*NOTE*
I have implemented that

Keywords cannot have some operators next to them.
Example:
- (list) is a valid input. 
- (and* is not a valid input. ( can come before or after a keyword however * cant. 
- ( ) " space or newline can come before or after a keyword

Before and after + - / * ** operators
- 3+5+input1 is valid
- and+or+exit is not valid
- (+ (3 5)) is valid
- ( ) " space newline identifier or value can come.
*********

*********
*NOTE*
I have implemented a few semantic rules for list keyword thinking it was needed as well.
Later I learnt it was not needed for the hw however, I have not removed that part from my code.
It does not prevent the lexer from doing it's job. Eventhough it encounters an error, it prints out the found
tokens in an error message.
-It lets list to have only values stored inside like it is mentioned in g++ syntax.
-Values can be divided by commas or spaces.
-List should have a ) at the end
-List can be empty. Ex: (list) or (list )
*********

2-) Lisp Lexer
-I have used portacle to compile it however, it should be compilable in clisp as well.
-This lexer can work with both files and regular user input.
-The directions to choose a file or user input is explained in the program.
-I removed the semantic rules I used for flex part from this lexer.
-Supports floating numbers as mentioned in g++ syntax.


*********
*NOTE*
I have provided an example input.txt and it's resulting output.txt for both lexers. 
input.txt can be edited.
*********