//Asaf Koenigsberg 318654118
//project minesweeper
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define ROWSEIGHT 8
#define COLSEIGHT 8
#define COLSTWELVE 12
#define ROWSTWELVE 12
#define ROWSFITHTEEN 15
#define COLSFITHTEEN 15
#define MAXROWS 22//max value of rows
#define MAXCOLS 22//max value of cols
#define UN_REVEALED 'X'//hides value behind
#define BOMB 1//yes bomb
#define NOBOMB 0//no bomb
#include <math.h>
typedef struct boardarea
{
	int bombOrNoBomb;
	char c;
}gameBoard;//a struct that represents one cell in the matrix.has a char value and an int that says if has bomb or not
void printBoard(gameBoard board[][MAXCOLS], int rows, int cols);//prints a board
void printMenu();//prints the menu
void initBoard(gameBoard board[][MAXCOLS], int rows, int cols);//initiates a board and puts the random bombs
void playGame(gameBoard board[][MAXCOLS], int rows, int cols);//plays an entire game.
bool isRightChoice(gameBoard board[][MAXCOLS], int rows, int cols, int chosenRow, int chosenCol, char action);//checks if the player chose a valid choice
void revealCells(gameBoard board[][MAXCOLS], int rows, int cols, int number);//reveals first number of cells which are not yet revealed and dont contain bombs
bool checkIfHitBomb(gameBoard board[][MAXCOLS], int chosenRow, int chosenCol);//checks if user hit bomb
void revealBoard(gameBoard board[][MAXCOLS], int rows, int cols);//reveals the entire board
int countAdjacentBombs(gameBoard board[][MAXCOLS], int rows, int cols, int selectedRow, int selectedCol);//counts how many bombs are near the chosen place
bool checkIfWon(gameBoard board[][MAXCOLS], int rows, int cols);//checks if the player won
bool checkIfChoiceInRange(int rows, int cols);//gets custom rows and cols,checks if 0<rows<=22 and 0<cols<=22
void revealNeighbors(gameBoard board[][MAXCOLS], int rows, int cols, int chosenRow, int chosenCol);//gets a board,rows,cols,chosen row and col.reveals all the neighbors of the cell
void main()
{
	int choice, rows, cols;//user chooses what to do	
	bool exit = false;//checks if the player wants to exit
	gameBoard board[MAXROWS][MAXCOLS];
	printMenu();
	scanf("%d", &choice);

	while (choice > 4 || choice < 0)
	{
		printf("Invalid choice\n");
		printMenu();
		scanf("%d", &choice);
	}
	switch (choice)
	{
	case 1:
		rows = ROWSEIGHT;
		cols = COLSEIGHT;
		break;
	case 2:
		rows = ROWSTWELVE;
		cols = COLSTWELVE;
		break;
	case 3:
		rows = ROWSFITHTEEN;
		cols = COLSFITHTEEN;
		break;
	case 4:
		printf("Please enter the size of the board (Lines<=22 and Cls<=22)\n");
		scanf("%d%d", &rows, &cols);
		while (!checkIfChoiceInRange(rows, cols))
		{
			printf("Choice is out of range. please enter again lines and cols");
			scanf("%d%d", &rows, &cols);
		}
		break;
	case 0:
		printf("Good bye");
		exit = true;
		break;
	}

	if (!exit)
	{
		initBoard(board, rows, cols);
		printBoard(board, rows, cols);
		playGame(board, rows, cols);
	}
}
void revealNeighbors(gameBoard board[][MAXCOLS], int rows, int cols, int chosenRow, int chosenCol)
{
	int adjacentBombs;
	if (board[chosenRow][chosenCol].c == UN_REVEALED||board[chosenRow][chosenCol].c=='F')
	{
		if (board[chosenRow][chosenCol].bombOrNoBomb == BOMB)
			return;
		adjacentBombs = countAdjacentBombs(board, rows, cols, chosenRow, chosenCol);
		if (adjacentBombs > 0)
		{
			board[chosenRow][chosenCol].c = adjacentBombs + '0';//converts from int to char
			return;
		}
		else
			board[chosenRow][chosenCol].c = ' ';//no bombs near

		if (chosenRow < rows - 1)
			revealNeighbors(board, rows, cols, chosenRow + 1, chosenCol);//go down
		if (chosenCol < cols - 1)
			revealNeighbors(board, rows, cols, chosenRow, chosenCol + 1);//go to the right
		if (chosenRow > 0)
			revealNeighbors(board, rows, cols, chosenRow - 1, chosenCol);//go up
		if (chosenCol > 0)
			revealNeighbors(board, rows, cols, chosenRow, chosenCol - 1);//go to the left
		//checks diagonals

		if (chosenRow < rows - 1 && chosenCol < cols - 1)
			revealNeighbors(board, rows, cols, chosenRow + 1, chosenCol + 1);
		if (chosenRow > 0 && chosenCol > 0)
			revealNeighbors(board, rows, cols, chosenRow - 1, chosenCol - 1);
		if (chosenRow < rows - 1 && chosenCol>0)
			revealNeighbors(board, rows, cols, chosenRow + 1, chosenCol - 1);
		if (chosenRow > 0 && chosenCol < cols - 1)
			revealNeighbors(board, rows, cols, chosenRow - 1, chosenCol + 1);
	}
}
bool checkIfChoiceInRange(int rows, int cols)
{
	if (rows > 22 || rows <= 0)
		return false;
	if (cols > 22 || cols <= 0)
		return false;
	return true;
}
void playGame(gameBoard board[][MAXCOLS], int rows, int cols)
{
	int chosenRow, chosenCol, adjacentBombs;
	char actionChoice;//O or F
	bool isLost = false, isWon = false;
	while (!isLost && !isWon)//while the game isnt over yet
	{
		printf("\n Please enter your move: row and col\n");
		scanf("%d%d", &chosenRow, &chosenCol);
		if (chosenRow != -1)//doesnt want to do the -1 action so we know he wants to open a single cell
		{
			scanf(" %c", &actionChoice);

			if (!isRightChoice(board, rows, cols, chosenRow, chosenCol, actionChoice))//scanf while the choice isnt right
			{
				printf("Please enter a valid choice.\n");
				continue;
			}
		}
		switch (chosenRow)
		{
			case -1:
				revealCells(board, rows, cols, chosenCol);//reveals the first col number that dont have bombs
				break;
			default:
			{
				switch (actionChoice)
				{
					case 'o':case 'O':
					{
						if (checkIfHitBomb(board, chosenRow, chosenCol))//player lost
						{
							isLost = true;
							revealBoard(board, rows, cols);
							break;
						}
						else
						{
							//reveals a single cell,whether it has a bomb near or not
							adjacentBombs = countAdjacentBombs(board, rows, cols, chosenRow, chosenCol);
							if (adjacentBombs == 0)
								revealNeighbors(board, rows, cols, chosenRow, chosenCol);//reveals the neighbors
							else
								board[chosenRow][chosenCol].c = adjacentBombs + '0';//convert from int to char
						}
						break;
					}
					case 'f':case 'F':
					{
						board[chosenRow][chosenCol].c = 'F';//if you want a flag,just put an F and break so that we go to the next iteration
						break;
					}
				}
			}
		}
		if (!isLost && checkIfWon(board, rows, cols))//player won
		{
			isWon = true;
			revealBoard(board, rows, cols);
		}
		printBoard(board, rows, cols);
	}

	if (isLost)
		printf("You've lost. game over...");
	else if (isWon)
		printf("You've won. congratulations");
}
bool checkIfWon(gameBoard board[][MAXROWS], int rows, int cols)
{
	int i, j;
	for (i = 0;i < rows;i++)
	{
		for (j = 0;j < cols;j++)
		{
			//if cell has no bomb but the cell is yet to be revealed (which means the game isnt over yet) or there is a flag in a place that doesnt have a bomb.
			if (board[i][j].bombOrNoBomb == NOBOMB && (board[i][j].c == UN_REVEALED || board[i][j].c == 'F'))
				return false;
		}
	}
	return true;//return true if only the bombs are unrevealed.
}
int countAdjacentBombs(gameBoard board[][MAXCOLS], int rows, int cols, int selectedRow, int selectedCol)
{
	int counter = 0;
	//checks all of the 8 neighbor cells of the current cell
	if (selectedRow - 1 >= 0 && selectedCol - 1 >= 0 && board[selectedRow - 1][selectedCol - 1].bombOrNoBomb == BOMB)
		counter++;
	if (selectedRow - 1 >= 0 && board[selectedRow - 1][selectedCol].bombOrNoBomb == BOMB)
		counter++;
	if (selectedRow - 1 >= 0 && selectedCol + 1 < cols && board[selectedRow - 1][selectedCol + 1].bombOrNoBomb == BOMB)
		counter++;
	if (selectedCol - 1 >= 0 && board[selectedRow][selectedCol - 1].bombOrNoBomb == BOMB)
		counter++;
	if (selectedCol + 1 < cols && board[selectedRow][selectedCol + 1].bombOrNoBomb == BOMB)
		counter++;
	if (selectedCol - 1 >= 0 && selectedRow + 1 < rows && board[selectedRow + 1][selectedCol - 1].bombOrNoBomb == BOMB)
		counter++;
	if (selectedRow + 1 < rows && board[selectedRow + 1][selectedCol].bombOrNoBomb == BOMB)
		counter++;
	if (selectedRow + 1 < rows && selectedCol + 1 < cols && board[selectedRow + 1][selectedCol + 1].bombOrNoBomb == BOMB)
		counter++;

	return counter;
}
bool checkIfHitBomb(gameBoard board[][MAXCOLS], int chosenRow, int chosenCol)
{
	return board[chosenRow][chosenCol].bombOrNoBomb == BOMB;
}
void revealBoard(gameBoard board[][MAXCOLS], int rows, int cols)
{
	int i, j, adjacentBombs;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (board[i][j].bombOrNoBomb == BOMB)//if there is a bomb puts * so we know that there is a bomb in that cell
				board[i][j].c = '*';
			else
			{
				adjacentBombs = countAdjacentBombs(board, rows, cols, i, j);//counts the number of the adjacent bombs in the current cell
				if (adjacentBombs > 0)
					board[i][j].c = adjacentBombs + '0';//converts from int to char
				else
					board[i][j].c = ' ';//no bombs near
			}
		}
	}
}
void revealCells(gameBoard board[][MAXCOLS], int rows, int cols, int number)
{
	int counter = number;
	int i, j, adjacentBombs;
	for (i = 0;i < rows && counter>0;i++)//while the counter isnt 0 and the row isnt over
	{
		for (j = 0;j < cols && counter>0;j++)//while the counter isnt 0 and the col isnt over
		{
			if (board[i][j].bombOrNoBomb == BOMB || board[i][j].c != UN_REVEALED)//if there is a bomb in that cell and its not revealed,continue to the next iteration
				continue;
			if (board[i][j].c == 'F')//if there is a flag then continue
				continue;
			adjacentBombs = countAdjacentBombs(board, rows, cols, i, j);//if there are bombs near the cell puts the number of bombs near.otherwise puts ' '
			if (adjacentBombs > 0)
				board[i][j].c = adjacentBombs + '0';
			else
				board[i][j].c = ' ';

			counter--;
		}
	}
}
bool isRightChoice(gameBoard board[][MAXCOLS], int rows, int cols, int chosenRow, int chosenCol, char action)
{
	if (chosenRow != -1 && chosenCol >= cols || chosenRow >= rows || chosenCol < 0 || chosenRow < -1)//-1 is a valid choice for a row or col
		return false;
	if (action != 'o' && action != 'O' && action != 'f' && action != 'F')
		return false;
	if (chosenRow >= 0 && board[chosenRow][chosenCol].c == 'F')//if there is a flag,so its allowed to be opened
		return true;
	if (chosenRow >= 0 && board[chosenRow][chosenCol].c != UN_REVEALED)//already revealed and not Flag
		return false;

	return true;
}
void printMenu()
{
	printf("Welcome to Minesweeper!\n\nPlease choose one of the following options and enter it's number:\n\n");
	printf("1 - for size 8x8\n\n2 - for size 12x12\n\n3 - for size 15x15\n\n4 - for custom size\n\n0 - Exit\n\n");
}

void initBoard(gameBoard board[][MAXCOLS], int rows, int cols)
{
	int randRow, randColumn, numOfBombs;
	srand(time(NULL));
	int i, j;
	for (i = 0;i < rows;i++)
	{
		for (j = 0;j < cols;j++)
		{
			board[i][j].c = UN_REVEALED;
			board[i][j].bombOrNoBomb = NOBOMB;//first we dont put a bomb in all of the cells
		}
	}
	numOfBombs = sqrt((int)rows * cols);
	for (i = 0;i < numOfBombs;i++)//we go to the loop num of bombs times in order to put a bomb in random places
	{
		randRow = rand() % rows;
		randColumn = rand() % cols;
		while (board[randRow][randColumn].bombOrNoBomb == BOMB)//check if a bomb is already planted.yes-enters the while loop and randomizes again
		{
			randRow = rand() % rows;
			randColumn = rand() % cols;
		}
		board[randRow][randColumn].bombOrNoBomb = BOMB;//plant bomb.0-no bomb,1=bomb
	}
}
void printBoard(gameBoard board[][MAXCOLS], int rows, int cols)
{
	int i, j;
	printf("  ");
	for (i = 0;i < cols;i++)
		printf("%4d", i);
	printf("\n   _");
	for (i = 0;i < cols;i++)
		printf("____");
	printf("\n");
	for (i = 0; i < rows; i++)
	{
		printf("%2d |", i);
		for (j = 0; j < cols; j++)
		{
			printf(" %c |", board[i][j].c);
		}
		printf("\n");
	}
	printf("   _");
	for (i = 0;i < cols;i++)
		printf("____");
}