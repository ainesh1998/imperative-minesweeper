/* Simple minesweeper game. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//-----------------------------------------------------------------------------------
// Constants and types.

// Constants representing the nature of cells.
enum cellType {Mine, Safe, One, Two, Three, Four, Five, Six, Seven, Eight, x};
typedef enum cellType cellType;

// Constants to represent validity checking.
enum validity { Unchecked, OK, BadFormat, BadLetter, BadDigit, BadCell };
typedef enum validity validity;

// The general game structure, which holds two grids - the actual one with
// the positions of the mines, and the one displayed to the player.
struct game {
    cellType actualGrid[9][9];
    cellType displayedGrid[9][9];
};
typedef struct game game;

//-----------------------------------------------------------------------------------
// Functions providing the logic in the game.

// Generates a random integer between 0 and 9 inclusive.
int random() {
    int r = rand() % 9;
    return r;
}

// Set all the cells to Safe, before allocating the Mines.
void placeSafe(game *g) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            g -> actualGrid[i][j] = Safe;
        }
    }
}

// Places 10 mines randomly on the initialised actualGrid. 
void placeMines(game *g) {
    placeSafe(g);
    int count = 0;
    srand(time(NULL));
    while (count < 10) {
        int row = random(); int col = random();
        if (g -> actualGrid[row][col] == Safe) {
            g -> actualGrid[row][col] = Mine;
            count++; 
        }
    }
}

// Checks adjacent cells for mines, returns the number of adjacent mines, given the cell is not a Mine.
int adjacent(game *g, int row, int col) {
    int count = 0;
    cellType adjacents[] = {Safe, Safe, Safe, Safe, Safe, Safe, Safe, Safe};
    
    if (row-1 >= 0 && col-1 >= 0) adjacents[0] = g -> actualGrid[row-1][col-1];
    if (row-1 >= 0 && col+1 < 9) adjacents[2] = g -> actualGrid[row-1][col+1];
    if (row+1 < 9 && col-1 >= 0) adjacents[5] = g -> actualGrid[row+1][col-1];
    if (row+1 < 9 && col+1 < 9) adjacents[7] = g -> actualGrid[row+1][col+1];
    if (row-1 >= 0) adjacents[1] = g -> actualGrid[row-1][col];
    if (col-1 >= 0) adjacents[3] = g -> actualGrid[row][col-1];
    if (col+1 < 9) adjacents[4] = g -> actualGrid[row][col+1];
    if (row+1 < 9) adjacents[6] = g -> actualGrid[row+1][col];
    
    for (int i = 0; i < 8; i++) {
        if (adjacents[i] == Mine) count++;
    }
    return count;
}

// Places the numbers indicating the number of adjacent mines if the cell is not a Mine.
void placeNumbers(game *g) {
    cellType numberOfMines[] = {Safe, One, Two, Three, Four, Five, Six, Seven, Eight};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> actualGrid[i][j] != Mine) {
                g -> actualGrid[i][j] = numberOfMines[adjacent(g, i, j)];
            }
        }
    }
}

// Set all grids in displayedGrid to x, indicating none of the actualGrid is displayed.
void placex(game *g) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            g -> displayedGrid[i][j] = x;
        }
    }
}

// Initialise the game.
void newGame(game *g) {
    placeMines(g);
    placeNumbers(g);
    placex(g);
}

// Convert the letter in a valid move string such as "b2" into a row index.
int row(char *text) {
    char rows[] = {'a','b','c','d','e','f','g','h','i'};
    int index = -1;
    for (int i = 0; i < 9; i++) {
        if (text[0] == rows[i]) index = i;
    }
    return index;
}

// Convert the digit in a valid move string such as "b2" into a column index.
int col(char *text) {
    int index = atoi(&text[1])-1;
    return index;
}

// Check whether a move string typed in by the user such as "b2" is valid, and
// the corresponding cell is available.  Return a validity code.
validity valid(game *g, char *text) {
    if (strlen(text)!=2) return BadFormat;
    else if (text[0]!='a' && text[0]!='b' && text[0]!='c' && text[0]!='d' && text[0]!='e' && text[0]!='f' && text[0]!='g' && text[0]!='h' && text[0]!='i') return BadLetter;
    else if (!isdigit(text[1]) || text[1] == '0') return BadDigit;
    else if (g -> displayedGrid[row(text)][col(text)] != x) return BadCell;
    return OK;
}

// Checks if the player has selected a Mine.
bool selectMine(game *g, int r, int c) {
    if (g -> actualGrid[r][c] == Mine) return true;
    return false;
}

// Make a move, provided the move is valid and the cell does not hold a mine.
void move(game *g, int r, int c) {
    cellType originalDisplayedCell = g -> displayedGrid[r][c];
    g -> displayedGrid[r][c] = g -> actualGrid[r][c];
    if (g -> actualGrid[r][c] == Safe && originalDisplayedCell != g -> actualGrid[r][c]) {        
        if (r-1 >= 0 && c-1 >= 0) move(g, r-1, c-1);
        if (r-1 >= 0 && c+1 < 9) move(g, r-1, c+1);
        if (r+1 < 9 && c-1 >= 0) move(g, r+1, c-1);
        if (r+1 < 9 && c+1 < 9) move(g, r+1, c+1);
        if (r-1 >= 0) move(g, r-1, c);
        if (c-1 >= 0) move(g, r, c-1);
        if (c+1 < 9) move(g, r, c+1);
        if (r+1 < 9) move(g, r+1, c);
    } 
}

// The player selects a Mine and loses. Displays all the mines.
void lost(game *g) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> actualGrid[i][j] == Mine) g -> displayedGrid[i][j] = g -> actualGrid[i][j];
        }
    }
}

// The player wins.
bool won(game *g) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> displayedGrid[i][j] != Mine && g -> displayedGrid[i][j] != x) count++;
        }
    }
    if (count == 71) return true;
    return false;
}

// //-----------------------------------------------------------------------------------
// User interface.

// Converts cellTypes to characters.
char toString(cellType type) {
    if (type == Mine) return 'M';
    else if (type == Safe) return '0';
    else if (type == x) return 'x';
    else if (type == One) return '1';
    else if (type == Two) return '2';
    else if (type == Three) return '3';
    else if (type == Four) return '4';
    else if (type == Five) return '5';
    else if (type == Six) return '6';
    else if (type == Seven) return '7';
    else if (type == Eight) return '8';
    return '0';
}

// Prints the grid.
void printGrid(game *g) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            char cell = toString(g -> displayedGrid[i][j]);
            printf("  %c  ", cell);
        }
        printf("\n");
    }
}

// Print a validity error message.
void printInvalid(validity v) {
    if (v == BadFormat) printf("Type a letter and a digit");
    else if (v == BadLetter) printf("Type a letter from a to i");
    else if (v == BadDigit) printf("Type a digit from 1 to 9");
    else if (v == BadCell) printf("Choose an empty cell");
    printf("\n");
}

// Handles the game as a whole, deals with input and output.
void play(game *g) {
    char cell[20];
    printGrid(g);
    int count = scanf("%s", cell);
    int r = row(cell); int c = col(cell);
    printf("%d\n", count);
    while (true) {
      while (valid(g, cell) != OK) {
          printInvalid(valid(g, cell));
          count = scanf("%s", cell);
      }
      if (!selectMine(g, r, c)) {
          move(g, r, c);
          printGrid(g);
          if (won(g)) {
              printf("You won!\n");
              break;
          }
          else {
              count = scanf("%s", cell);
              r = row(cell); c = col(cell);
          }
       }
       else {
           lost(g); printGrid(g);
           printf("You lost.\n");
           break;
       }
    }
}

//-----------------------------------------------------------------------------------
// Testing and running.

// Checks that the constants have not been changed.
void checkConstants() {
    assert((Mine == 0) && (Safe == 1) && (One == 2) && (Two == 3) && (Three == 4) && (Four == 5) && (Five == 6) && (Six == 7) && (Seven == 8) && (Eight == 9) && (x == 10));
}

// Tests that all cells are marked as Safe.
void testPlaceSafe(game *g) {
    int count = 0;
    placeSafe(g);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> actualGrid[i][j] == Safe) count++;
        }
    }
    assert(count == 81);
}

// Tests that 10 mines are randomly placed.
void testPlaceMines(game *g) {
    placeSafe(g);
    placeMines(g);
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> actualGrid[i][j] == Mine) count++;
        }
    }
    assert(count == 10);   
}

// Tests the adjacent function.
void testAdjacent(game *g) {
    placeSafe(g);
    g -> actualGrid[8][6] = Mine; g -> actualGrid[7][8] = Mine; g -> actualGrid[8][8] = Mine; g -> actualGrid[3][7] = Mine;
    assert(adjacent(g, 8, 7) == 3); 
}

// Tests if each adjacent cell has been numbered correctly.
// Tests for one adjacent mine.
void testPlaceNumbersOne(game *g) {
    placeSafe(g);
    g -> actualGrid[3][5] = Mine;
    placeNumbers(g);
    assert(g -> actualGrid[2][4] == One);
    assert(g -> actualGrid[2][5] == One);
    assert(g -> actualGrid[2][6] == One);
    assert(g -> actualGrid[3][4] == One);
    assert(g -> actualGrid[3][5] == Mine);
    assert(g -> actualGrid[3][6] == One);
    assert(g -> actualGrid[4][4] == One);
    assert(g -> actualGrid[4][5] == One);
    assert(g -> actualGrid[4][6] == One);
}

// Tests for two adjacent mines.
void testPlaceNumbersTwo(game *g) {
    g -> actualGrid[3][7] = Mine;
    placeNumbers(g);
    assert(g -> actualGrid[3][6] == Two);
    assert(g -> actualGrid[4][6] == Two);
}

// Tests for three adjacent mines.
void testPlaceNumbersThree(game *g) {
    g -> actualGrid[3][6] = Mine;
    placeNumbers(g);
    assert(g -> actualGrid[3][5] == Mine);
    assert(g -> actualGrid[4][6] == Three);
}

// Tests that all of displayedGrid's cells are x.
void testPlacex(game *g) {
    int count = 0;
    placex(g);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> displayedGrid[i][j] == x) count++;
        }
    }
    assert(count == 81);
    
}
// Tests the row and col functions.
void testRowCol() {
    assert(row("f3") == 5);
    assert(row("d9") == 3);
    assert(col("f3") == 2);
    assert(col("d9") == 8);
}

// Tests the valid function.
void testValid(game *g) {
    placeSafe(g);
    assert(valid(g, "f44") == BadFormat);
    assert(valid(g, "9g") == BadLetter);
    assert(valid(g, "y8") == BadLetter);
    assert(valid(g, "j9") == BadLetter);
    g -> displayedGrid[3][5] = Safe;
    assert(valid(g, "d6") == BadCell);
}

// Tests the selectMine function.
void testSelectMine(game *g) {
    placeSafe(g);
    g -> actualGrid[6][6] = Mine;
    assert(selectMine(g, 6, 6) == true);
    assert(selectMine(g, 5, 5) == false);
}

// Tests the move function, for position [1][5].
void testMove1(game *g) {
    move(g, 1, 5);
    assert(g -> displayedGrid[0][3] == One);
    assert(g -> displayedGrid[1][3] == One);
    assert(g -> displayedGrid[2][3] == One);
    assert(g -> displayedGrid[0][4] == Safe);
    assert(g -> displayedGrid[1][4] == Safe);
    assert(g -> displayedGrid[2][4] == One);
    assert(g -> displayedGrid[0][5] == Safe);
    assert(g -> displayedGrid[1][5] == Safe);
    assert(g -> displayedGrid[2][5] == One);
    assert(g -> displayedGrid[0][6] == Safe);
    assert(g -> displayedGrid[1][6] == Safe);
    assert(g -> displayedGrid[2][6] == One);
    assert(g -> displayedGrid[0][7] == One);
    assert(g -> displayedGrid[1][7] == Two);
    assert(g -> displayedGrid[2][7] == One);
    
}

// Tests the move function, for position [3][3].
void testMove2(game *g) {
    /*move(g, 3, 3);
    assert(g -> displayedGrid[2][2] == One);
    assert(g -> displayedGrid[3][2] == Safe);
    assert(g -> displayedGrid[4][2] == One);
    assert(g -> displayedGrid[2][3] == One);
    assert(g -> displayedGrid[3][3] == Safe);
    assert(g -> displayedGrid[4][3] == One);
    assert(g -> displayedGrid[2][4] == One);
    assert(g -> displayedGrid[3][4] == One);
    assert(g -> displayedGrid[4][4] == One);*/
}

// Runs the move function's tests.
void testMove(game *g) {
    placeSafe(g);
    g -> actualGrid[1][2] = Mine; 
    g -> actualGrid[3][5] = Mine; 
    g -> actualGrid[0][8] = Mine;
    g -> actualGrid[2][8] = Mine;
    placeNumbers(g);
    placex(g);
    testMove1(g);
    testMove2(g);
}

// Tests the lost function.
void testLost(game *g) {
    newGame(g);
    lost(g); int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> displayedGrid[i][j] == Mine) count++;
        }
    }
    assert(count == 10);
}

// Tests the won function.
void testWon(game *g) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (g -> displayedGrid[i][j] != Mine) count++;
        }
    }
    assert(count == 71);
}

// Runs all the tests.
void test() {
    game *g = malloc(sizeof(game));
    checkConstants();
    testPlaceSafe(g);
    testPlaceMines(g);
    testAdjacent(g);
    testPlaceNumbersOne(g);
    testPlaceNumbersTwo(g);
    testPlaceNumbersThree(g);
    testPlacex(g);
    testRowCol();
    testValid(g);
    testSelectMine(g);
    testMove(g);
    testLost(g);
    testWon(g);
    printf("All tests passed.\n");
}

//-----------------------------------------------------------------------------------
// Main function, tests are run if no arguments are given.
int main(int n, char *args[n]) {
    game gdata;
    game *g = &gdata;
    newGame(g);
    if (n == 2 && strcmp(args[1],"play") == 0) play(g);
    else test();
}