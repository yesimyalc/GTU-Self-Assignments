I have not found any errors myself but if you want to remove the SavedGame.txt files by hand
and do not remove in an order, for example if you remove SavedGame1.txt but leave SavedGame2.txt, 
the program's load&save mechanism will not work as intended. But if you remove SavedGame2.txt and
do not remove SavedGame1.txt it will work properly. If you remove all, again it will work properly.

The reason for that is, I made the user save games in an order. The user cannot create a 
SavedGames2.txt if they have not created SavedGames1.txt before. However, this rule could be
broken if they were to delete or create new SavedGame.txt files outside of the program by hand.