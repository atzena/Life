// Life simulation
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

const int MAXWORLDROWS = 25;
const int MAXWORLDCOLS = 80;
const int MAXROWS = MAXWORLDROWS + 2;
const int MAXCOLS = MAXWORLDCOLS + 2;
const int FILENAMESIZE = 255;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
// for interesting effect use code 219 and 176 instead of the asterisk and space
const unsigned char CELL = '*';  // 219;
const unsigned char EMPTY = ' '; // 176;

enum class State { dead = 0, alive = 1 };
/*
* initWorld is used to initialize all positions in the world
* array to the 'dead' state
*/
void initWorld(State world[][MAXCOLS]);

/*
* display is used to draw the 'world' onto the screen
* for each 'alive' cell, the value CELL is displayed
* for each 'dead' cell, the value EMPTY is displayed
* does NOT display any items in:
*   row 0, row MAXROWS-1, column 0, or column MAXCOLS-1
* since these rows and columns are used as a "border"
* around the world grid
*/

void display(State world[][MAXCOLS]);

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and update the world
* based on the contents of the file. The file should contain
* only spaces and asterisks. A space represents a 'dead' cell
* and an asterisk represents an 'alive' cell.
* The function stops reading and exits the program if > MAXWORLDCOLS
* characters are found on one row, or if > MAXWORLDROWS rows are
* found in the file.
*/
void getFile(State world[][MAXCOLS]);

/*
* calcNewWorld is used to calculate the new state of the world
* (i.e. a new generation) based upon analyzing the current world state
*/
void calcNewWorld(State world[][MAXCOLS]);

/*
* neighborCount is used to count the number of 'alive' cells in
* the 8 neighboring positions which are surrounding position
* [row][col] in world
*/
int neighborCount(State world[][MAXCOLS], int row, int col);

int main()
{
	State world[MAXROWS][MAXCOLS];
	int generation = 0;
	bool stop = false;



	// Write initWorld & getFile outside of the do while loop


	initWorld(world);
	getFile(world);




	/*do while loop dislays simulation.
	Promts user for inputs on how to proceed with program.
	Clears prior simulation and displays new cells or ends upon users request.*/
	do
	{
		// system("cls") clears the screen; only works on Windows machines
		// You will probably need to comment this out if working on
		// a non-Windows machine
		system("cls");

		display(world);
		generation++;
		cout << "Generation: " << generation << "\t";
		cout << "<ENTER> to continue / <Q + ENTER> to quit: ";

		char input[5];
		cin.getline(input, 5);   //Gets the users input by using getline so it reads the enter button.


		if (input[0] == 'q')
		{
			stop = true;
		}
		else {
			calcNewWorld(world);
		}



	} while (!stop);


	return 0;
}

/*This function finds the number of neighbors surrounding each index so that the calcNewWorld function can
determine whether or not the cell is alive or empty depending on the number of neighbors*/
int neighborCount(State world[][MAXCOLS], int row, int col)
{
	// This code checks to make sure you aren't trying to count neighbors for any
	// position on row #0, or on row #MAXWORLDROWS+1,
	// or in column 0, or in columnn MAXWORLDCOLUMNS+1
	// because if you are, you are going to run into array out-of-bounds problems
	if (row < 1 || row > MAXWORLDROWS)
	{
		cerr << "ERROR: invalid row parameter " << row << " in neighborCount\n";
		exit(1);
	}
	else if (col < 1 || col > MAXWORLDCOLS)
	{
		cerr << "ERROR: invalid col parameter " << col << " in neighborCount\n";
		exit(1);
	}
	// TODO: write neighborCount code

	int i = 0;


	i = i + (int)world[row - 1][col - 1];
	i = i + (int)world[row][col - 1];
	i = i + (int)world[row][col + 1];
	i = i + (int)world[row - 1][col];
	i = i + (int)world[row + 1][col - 1];
	i = i + (int)world[row + 1][col];
	i = i + (int)world[row + 1][col + 1];
	i = i + (int)world[row - 1][col + 1];


	return i;
}

// This function uses the neighborcount for each spot in the array to determine what rules to apply
// World array is initilized and filled with zeros
//The states are stored in the world copy array and then the worldcopy array values are assigned to the world array
void calcNewWorld(State world[][MAXCOLS])
{
	State worldCopy[MAXROWS][MAXCOLS];
	int count = 0;

	initWorld(worldCopy);  //Fills the copy world with dead states

	//These loops go through the indexs and changes the states based off how many neighbors the index has.
	for (int row = 1; row < MAXWORLDROWS; row++)
	{
		for (int col = 1; col <= MAXWORLDCOLS; col++)
		{
			count = neighborCount(world, row, col);
			if (count == 2)
			{
				worldCopy[row][col] = world[row][col];
			}
			else if (count == 3)
			{
				worldCopy[row][col] = State::alive;
			}
			else {
				worldCopy[row][col] = State::dead;
			}
		}
	}

	//This loop copies the contents of the world copy array into the world array
	for (int row = 1; row <= MAXWORLDROWS; row++)
	{
		for (int col = 1; col <= MAXWORLDCOLS; col++)
		{
			world[row][col] = worldCopy[row][col];
		}
	}
}


//this function gets the file contents by reading by character and copies the contents into the world array
void getFile(State world[][MAXCOLS])
{

	ifstream inFile;
	int row = 1; //starter code


	cout << "Enter the filename containing the initial structure: \n";


	char nameFile[FILENAMESIZE];  //name of file
	cin.getline(nameFile, FILENAMESIZE);     //prompts user to open a file
	inFile.open(nameFile);  //Opens the file

	if (!inFile) // Checks to see if the file works
	{
		cerr << "ERROR: input file could not be opened.";
		exit(1);   //Exits the program if file fails to open
	}

	//reads by character from the file
	char c = inFile.get();

	// makes sure that the program doesnt go off the end of the file and keeps going until it reaches the end of the row
	// or the end of the column. The function also gets and stores the characters.
	// for loop stops incramenting rows if too many characters are on a line
	bool tooManyCh = false;
	for (row; !inFile.eof() && row <= MAXWORLDROWS && !tooManyCh; row++)
	{
		for (int col = 1; !inFile.eof() && col <= MAXWORLDCOLS && c != '\n'; col++)
		{
			if (c == CELL)
			{
				world[row][col] = State::alive;
			}
			else if (c == EMPTY)
			{
				world[row][col] = State::dead;
			}
			c = inFile.get();
		}
		if (c != '\n')
		{
			tooManyCh = true;
			row--;
		}
		else {
			c = inFile.get();
		}
	}

	// TODO: Write getFile code
	// Make sure row is incremented each time you read a line from the file

	// After file reading is finished, this code checks to see if the reading stopped
	// because of too many characters on a line in the file, or too many rows in the file
	// It is possible that you might have to change the conditions here depending on
	// exactly how you choose to read the file

	if (!inFile.eof())
	{
		if (row <= MAXWORLDROWS)
		{
			cerr << "\nERROR: Line " << row << " in input file contains more than " << MAXWORLDCOLS << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXWORLDROWS << " lines in the file\n";
		}
		inFile.close();
		exit(1);
	}
	inFile.close();

}

//This function prints the output as either an asterick or a empty space depending on the state. If the value is dead then it prints an empty space. 
//If the value is alive then it prints an asterick. 
void display(State world[][MAXCOLS]) {

	// Counts each row 
	for (int row = 1; row <= MAXWORLDROWS; row++)
	{
		// Counts each column
		for (int col = 1; col <= MAXWORLDCOLS; col++)
		{
			// An alternative to an if statement to check for two possible states: dead or alive
			switch (world[row][col])
			{
				// If dead, the function prints EMPTY, which represents " "
			case State::dead:

				cout << EMPTY;

				break;
				// If alive, the function prints CELL, which represents "*"
			case State::alive:

				cout << CELL;

				break;
			}
		}

		cout << endl;
	}
}

// This function fills the given array completely with dead states.
void initWorld(State world[][MAXCOLS]) {

	for (int rows = 0; rows < MAXROWS; rows++) {

		for (int cols = 0; cols < MAXCOLS; cols++) {

			world[rows][cols] = State::dead;
		}
	}
}




