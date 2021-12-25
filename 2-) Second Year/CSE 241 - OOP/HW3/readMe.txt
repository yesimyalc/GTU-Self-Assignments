*****How to Enter Commands*****
-You can determine your move by entering cell coordinates. (example: F1, B1, A5)
-If you want to save or load the game at any point,
you can use LOAD FILE.TXT and SAVE FILE.TXT commands when you are asked to make your move.
-You can create more than one games at a time and can move between them.
-To move between games, you can use MENU command when you are asked to make your move.
-To compare the current game with another game, you can use COMPARE command when you are asked to make your move.

********************************
The same things above are written at the beginning of the game too, but I am putting these instructions as a reminder.

Notes:

According to my understanding from the homework pdf, I made these functions like this:
-In my compareGames function,
I compared the marked cells for the user who has the current turn. 
For example if the turn is at User 1 who is using X, the cells which has X inside will be counted.
If the turn is at User 2 who is using O, the cells which has O inside will be counted. 
More detailed information is also written in the game. 

-The function that returns allMarkedCells in my code, returns the total marked cells in all games.
It does not return for each game seperately.

-There is no limit for the board size, but like it was mentioned before it probably does not work as 
intended if the board size is too big. For example the board cannot be displayed properly because it does not fit the screen.

-When it is asked to make a move from the user, if the user enters a really big number randomly as input instead
of a cell coordinate, the code will not work as intended because the stoi function cannot copy paste that large number correctly