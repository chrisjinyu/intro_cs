#ifndef HISTORY_H			//if not defined beforehand (guards against extra)
#define HISTORY_H

#include "globals.h"		//required to implement a proper display grid size using constants MAXROWS, MAXCOLS

class History
{
public:
	History(int nRows, int nCols);		//no default constructor allowed
	bool record(int r, int c);
	void display() const;

private:
	int m_rows;
	int m_cols;
	int m_grid[MAXROWS][MAXCOLS];	//integer grid to keep track of number of records
};

#endif						//end guard