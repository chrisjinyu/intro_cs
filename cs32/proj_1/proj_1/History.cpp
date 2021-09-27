#include "History.h"			//include header file
#include <iostream>				//required to do proper cout

using namespace std;			//required to execute iostream

History::History(int nRows, int nCols) :m_rows(nRows), m_cols(nCols)	//assign private member variables with constructed values
{
	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS; j++)
			m_grid[i][j] = 0;					//ensure grid is filled with 0
}

bool History::record(int r, int c)
{
	if (r<1 || r>MAXROWS || c<1 || c>MAXCOLS)		//false condition, invalid recording state
		return false;
	m_grid[r - 1][c - 1]++;							//increment by 1 to notify History object that pellet has been eaten here
	return true;									//ran with no error
}

void History::display() const
{
	char displayGrid[MAXROWS][MAXCOLS];				//new displayGrid to eventually display
	int r, c;										//for saved memory

	// Fill displayGrid with dots
	for (r = 0; r < m_rows; r++)
		for (c = 0; c < m_cols; c++)
		{
			if (m_grid[r][c] <= 0)
				displayGrid[r][c] = '.';			//all blanks are '.'
			else if (m_grid[r][c] >= 26)
				displayGrid[r][c] = 'Z';			//all >=26 are 'Z'
			else
				displayGrid[r][c] = 'A' - 1 + m_grid[r][c];		//all in between 0 and 26 are 'A' through 'Y'
		}

	clearScreen();

	for (r = 0; r < m_rows; r++)
	{
		for (c = 0; c < m_cols; c++)
			cout << displayGrid[r][c];				//print out array by array in int[][]
		cout << endl;
	}
	cout << endl;
	//no cout << "Press enter to continue." line
}