# imperative-minesweeper
Imperative Programming OXO coursework open-end

This program is a simple minesweeper simulator in the terminal. 
- When "./minesweeper play" is called in the terminal, the game is started.

- A cell is played when its coordinates are entered. If an invalid cell coordinate is entered, an error message is printing, prompting the player for another input. 

- The player loses when they select a Mine. The current grid is printed out, with all Mines shown.

- The player wins when all the cells have been selected except the Mines.

- The program is split into logic, interface, then testing.

- Two grids are stored in struct game - actualGrid(the grid behind what the player sees, which holds the mines and the number of mines adjacent to each cell), and displayedGrid(the grid shown to the player). As the game progresses, displayedGrid's cells are set to the corresponding cell in actualGrid after the cell has been displayed.

- The game is first initialised by setting all cells in actualGrid to Safe, then randomly placing 10 Mines, then placing the number of mines adjacent to each cell in the cell, then setting all cells in displayedGrid to x.

- The adjacent cell returns the number of Mines adjacent to the given cell.

- The move function uses recursion to cause the rippling effect we see in Minesweeper when the cells are displayed.

- The play function is the main function that drives the program. It sets up a while loop to keep the game going, and checks along the way that the coordinates given are valid.

- Tests are run when no arguments are given to main, as usual.

There is a problem with this program, however. After a while, the cell chosen by the player is not what is displayed by the program. Many random cells are displayed, too. I am not sure what the problem is, and have not had the time to debug it. 
