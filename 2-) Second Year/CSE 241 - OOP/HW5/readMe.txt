*****How to Enter Commands*****
-You can determine your move by entering cell coordinates. (example: F1, B1, A5)
-If you want to save or load the game at any point,
you can use LOAD and SAVE commands when you are asked to make your move.
-You can create more than one games at a time and can move between them.
-To move between games, you can use MENU command when you are asked to make your move.
-To compare the current game with another game, you can use COMPARE command when you are asked to make your move.
-To exit from all games, you can use EXIT command.
-To edit the game, you can enter CHANGE command.
-To reset a game, you can use RESET command.
-To see the last made move, you can use LAST command.
********************************
The same things above are written at the beginning of the game too, but I am putting these instructions as a reminder.



Explanation of My Driver Codes:

HexVectorGame: Fully functioning hex game program that uses HexVector class. It uses all the functions HexVector class is supposed to use like a driver code too.
HexArray1DGame: Fully functioning hex game program that uses HexArray1D class. It uses all the functions HexArray1D class is supposed to use like a driver code too.
HexAdapterGame: Fully functioning hex game program that uses HexAdapter class. It uses all the functions HexAdapter class is supposed to use like a driver code too.

!!!Homework's Main Driver Code Here!!!
mainHexGame: This program is for testing the globalFunctions with all the hex board types mainly. It's main aim is to create hex boards to your liking and when you
are done with creating&editing the boards as you like, it sends them to the globalFunctions. Because in the homework we are asked to
test the globalFunctions at least 5 times, it is in a loop that will repeat 5 times. You can also fully play hex games in this program while creating the boards.

Notes:

-I have 2 global functions. One named is_validSequence, the other one is named is_validGame.
is_validSequence: Checks if the given array of hex games' moves can create a valid sequence of moves for a game. (That's what I have understood from the homework PDF)
is_validGame: Checks if the given array of hex games all have valid moves and valid move structures. (That's what the email answers said)
I could not understand which one the homework wants from us after some of the emails answers, that's why I implemented both.

-My lastMove() function returns the Cell the last move was done. This cell holds all the information of the last move. (ex: row, column, state...)

Possible Errors:

-When it is asked to make a move from the user, if the user enters a really big number randomly as input instead
of a cell coordinate, the code will not work as intended because the stoi function cannot copy paste that large number correctly