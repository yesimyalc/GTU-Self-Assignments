*****How to Enter Commands*****
-You can determine your move by entering cell coordinates. (example: F1, B1, A5)
-If you want to save or load the game at any point,
you can use LOAD FILE.TXT and SAVE FILE.TXT commands when you are asked to make your move.
-You can create more than one games at a time and can move between them.
-To move between games, you can use MENU command when you are asked to make your move.
-To compare the current game with another game, you can use COMPARE command when you are asked to make your move.
-To reset a move enter --UNDO or UNDO-- commands.
-To see your score, use SCORE command.

********************************
The same things above are written at the beginning of the game too, but I am putting these instructions as a reminder.

Notes:

According to my understanding from the homework pdf, I made these functions like this:
-In my == operation overload function,
I compared the marked cells for the user who has the current turn. 
For example if the turn is at User 1 who is using X, the cells which has X inside will be counted.
If the turn is at User 2 who is using O, the cells which has O inside will be counted. 
More detailed information is also written in the game. 
It is the same as my old compareGames function according to the homework instructions

-My UNDO functions work like this:
If the gameType is user vs user, deletes the latest move only and returns the turn to whoever's move was deleted.
If the gameType is user vs computer, deletes the two latest moves. 1 from the computer and 1 from the user. Because
if only the computer's move is deleted, the computer will make the same move again and again.

-I calculate the scores in my previous neighbourCheck function because I already track down all the connected cell's there.
I did not want to create another function that does almost the same thing, thinking it would be a big copy paste. 
However I have implemented another function that returns the wanted score that was calculated in neighbourCheck. 

Possible Errors:

-When it is asked to make a move from the user, if the user enters a really big number randomly as input instead
of a cell coordinate, the code will not work as intended because the stoi function cannot copy paste that large number correctly